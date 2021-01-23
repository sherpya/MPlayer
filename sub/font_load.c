/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libavutil/intreadwrite.h"
#include "libavutil/avstring.h"

#include "font_load.h"
#include "sub.h"
#include "mp_msg.h"

raw_file* load_raw(char *name,int verbose){
    int bpp;
    unsigned size;
    raw_file* raw=calloc(1, sizeof(*raw));
    unsigned char head[32];
    FILE *f=fopen(name,"rb");
    if(!f) goto err_out;                        // can't open
    if(fread(head,32,1,f)<1) goto err_out;        // too small
    if(memcmp(head,"mhwanh",6)) goto err_out;        // not raw file
    raw->w=AV_RB16(head + 8);
    raw->h=AV_RB16(head + 10);
    raw->c=AV_RB16(head + 12);
    if(raw->w == 0) // 2 bytes were not enough for the width... read 4 bytes from the end of the header
    	raw->w = AV_RB32(head + 28);
    if(raw->c>256) goto err_out;                 // too many colors!?
    if (!raw->w || !raw->h ||
        raw->w > INT_MAX / 4 || raw->h > INT_MAX / 4 / raw->w)
        goto err_out;
    mp_msg(MSGT_OSD, MSGL_DBG2, "RAW: %s  %d x %d, %d colors\n",name,raw->w,raw->h,raw->c);
    if(raw->c){
        raw->pal=calloc(raw->c, 3);
        if (fread(raw->pal,3,raw->c,f) != raw->c)
          goto err_out;
        bpp=1;
    } else {
        bpp=3;
    }
    size = raw->h*raw->w*bpp;
    raw->bmp=malloc(size);
    if (fread(raw->bmp,1,size,f) != size) {
        goto err_out;
    }
    fclose(f);
    return raw;

err_out:
    if (f)
      fclose(f);
    if (raw) {
      free(raw->pal);
      free(raw->bmp);
    }
    free(raw);
    return NULL;
}

font_desc_t* read_font_desc(const char* fname,float factor,int verbose){
unsigned char sor[1024];
unsigned char sor2[1024];
FILE *f = NULL;
char *dn;
//struct stat fstate;
char section[64];
int i,j;
int chardb=0;
int fontdb=-1;
int first=1;
int unicode;

font_desc_t *desc=calloc(1, sizeof(*desc));
if(!desc) goto fail_out;

f=fopen(fname,"rt");if(!f){ mp_msg(MSGT_OSD, MSGL_V, "font: can't open file: %s\n",fname); goto fail_out;}

i = strlen (fname) - 9;
if ((dn = malloc(i+1))){
   strncpy (dn, fname, i);
   dn[i]='\0';
}

desc->fpath = dn; // search in the same dir as fonts.desc

// desc->fpath=get_path("font/");
// if (stat(desc->fpath, &fstate)!=0) desc->fpath=DATADIR"/font";




// set up some defaults, and erase table
desc->charspace=2;
desc->spacewidth=12;
desc->height=0;
for(i=0;i<65536;i++) desc->start[i]=desc->width[i]=desc->font[i]=-1;

section[0]=0;

unicode = !subtitle_font_encoding || av_strcasecmp(subtitle_font_encoding, "unicode") == 0;

while(fgets(sor,1020,f)){
  unsigned char* p[8];
  int pdb=0;
  unsigned char *s=sor;
  unsigned char *d=sor2;
  int ec=' ';
  int id=0;
  sor[1020]=0;

  /* skip files that look like: TTF (0x00, 0x01), PFM (0x00, 0x01), PFB
   * (0x80, 0x01), PCF (0x01, 0x66), fon ("MZ"), gzipped (0x1f, 0x8b) */

  if (first) {
    if (!sor[0] || sor[1] == 1 || (sor[0] == 'M' && sor[1] == 'Z') || (sor[0] == 0x1f && sor[1] == 0x8b) || (sor[0] == 1 && sor[1] == 0x66)) {
      mp_msg(MSGT_OSD, MSGL_ERR, "%s doesn't look like a bitmap font description, ignoring.\n", fname);
      goto fail_out;
    }
    first = 0;
  }

  p[0]=d;++pdb;
  while(1){
      int c=*s++;
      if(c==0 || c==13 || c==10) break;
      if(!id){
        if(c==39 || c==34){ id=c;continue;} // idezojel
        if(c==';' || c=='#') break;
        if(c==9) c=' ';
        if(c==' '){
          if(ec==' ') continue;
          *d=0; ++d;
          p[pdb]=d;++pdb;
          if(pdb>=8) break;
          continue;
        }
      } else {
        if(id==c){ id=0;continue;} // idezojel

      }
      *d=c;d++;
      ec=c;
  }
  if(d==sor2) continue; // skip empty lines
  *d=0;

//  printf("params=%d  sor=%s\n",pdb,sor);
//  for(i=0;i<pdb;i++) printf("  param %d = '%s'\n",i,p[i]);

  if(pdb==1 && p[0][0]=='['){
      int len=strlen(p[0]);
      if(len && len<63 && p[0][len-1]==']'){
        strcpy(section,p[0]);
        mp_msg(MSGT_OSD, MSGL_DBG2, "font: Reading section: %s\n",section);
        if(strcmp(section,"[files]")==0){
            ++fontdb;
            if(fontdb>=16){ mp_msg(MSGT_OSD, MSGL_ERR, "font: Too many bitmaps defined.\n");goto fail_out;}
        }
        continue;
      }
  }

  if(strcmp(section,"[fpath]")==0){
      if(pdb==1){
          free (desc->fpath); // release previously allocated memory
          desc->fpath=strdup(p[0]);
          continue;
      }
  } else

#ifdef __AMIGAOS4__
#define FONT_PATH_SEP ""
#else
//! path seperator for font paths, may not be more than one character
#define FONT_PATH_SEP "/"
#endif

  if(strcmp(section,"[files]")==0){
      char *default_dir=MPLAYER_DATADIR FONT_PATH_SEP "font";
      if(pdb==2 && strcmp(p[0],"alpha")==0){
    	  char *cp;
	  if (!(cp=malloc(strlen(desc->fpath)+strlen(p[1])+2))) goto fail_out;

	  snprintf(cp,strlen(desc->fpath)+strlen(p[1])+2,"%s" FONT_PATH_SEP "%s",
		desc->fpath,p[1]);
          if(!((desc->pic_a[fontdb]=load_raw(cp,verbose)))){
		free(cp);
		if (!(cp=malloc(strlen(default_dir)+strlen(p[1])+2)))
		   goto fail_out;
		snprintf(cp,strlen(default_dir)+strlen(p[1])+2,"%s" FONT_PATH_SEP "%s",
			 default_dir,p[1]);
		if (!((desc->pic_a[fontdb]=load_raw(cp,verbose)))){
		   mp_msg(MSGT_OSD, MSGL_ERR, "Can't load font bitmap: %s\n",p[1]);
		   free(cp);
		   goto fail_out;
		}
          }
	  free(cp);
          continue;
      }
      if(pdb==2 && strcmp(p[0],"bitmap")==0){
    	  char *cp;
	  if (!(cp=malloc(strlen(desc->fpath)+strlen(p[1])+2))) goto fail_out;

	  snprintf(cp,strlen(desc->fpath)+strlen(p[1])+2,"%s" FONT_PATH_SEP "%s",
		desc->fpath,p[1]);
          if(!((desc->pic_b[fontdb]=load_raw(cp,verbose)))){
		free(cp);
		if (!(cp=malloc(strlen(default_dir)+strlen(p[1])+2)))
		   goto fail_out;
		snprintf(cp,strlen(default_dir)+strlen(p[1])+2,"%s" FONT_PATH_SEP "%s",
			 default_dir,p[1]);
		if (!((desc->pic_b[fontdb]=load_raw(cp,verbose)))){
		   mp_msg(MSGT_OSD, MSGL_ERR, "Can't load font bitmap: %s\n",p[1]);
		   free(cp);
		   goto fail_out;
		}
          }
	  free(cp);
          continue;
      }
  } else

  if(strcmp(section,"[info]")==0){
      if(pdb==2 && strcmp(p[0],"name")==0){
          desc->name=strdup(p[1]);
          continue;
      }
      if(pdb==2 && strcmp(p[0],"descversion")==0){
          // This version field was never used.
          // Since bitmap fonts are now deprecated there is
          // no point in handling it.
          continue;
      }
      if(pdb==2 && strcmp(p[0],"spacewidth")==0){
          desc->spacewidth=atoi(p[1]);
          continue;
      }
      if(pdb==2 && strcmp(p[0],"charspace")==0){
          desc->charspace=atoi(p[1]);
          continue;
      }
      if(pdb==2 && strcmp(p[0],"height")==0){
          desc->height=atoi(p[1]);
          continue;
      }
  } else

  if(strcmp(section,"[characters]")==0){
      if(pdb==3){
          int chr=p[0][0];
          int start=atoi(p[1]);
          int end=atoi(p[2]);
          if(unicode && (chr>=0x80)) chr=(chr<<8)+p[0][1];
          else if(strlen(p[0])!=1) chr=strtol(p[0],NULL,0);
          if(end<start) {
              mp_msg(MSGT_OSD, MSGL_WARN, "error in font desc: end<start for char '%c'\n",chr);
          } else {
              desc->start[chr]=start;
              desc->width[chr]=end-start+1;
              desc->font[chr]=fontdb;
//              printf("char %d '%c'  start=%d width=%d\n",chr,chr,desc->start[chr],desc->width[chr]);
              ++chardb;
          }
          continue;
      }
  }
  mp_msg(MSGT_OSD, MSGL_ERR, "Syntax error in font desc: %s",sor);
  goto fail_out;

}
fclose(f);
f = NULL;

 if (first == 1) {
   mp_msg(MSGT_OSD, MSGL_ERR, "%s is empty or a directory, ignoring.\n", fname);
   goto fail_out;
 }

//printf("font: pos of U = %d\n",desc->start[218]);

for(i=0;i<=fontdb;i++){
    if(!desc->pic_a[i] || !desc->pic_b[i]){
        mp_msg(MSGT_OSD, MSGL_ERR, "font: Missing bitmap(s) for sub-font #%d\n",i);
        goto fail_out;
    }
    //if(factor!=1.0f)
    {
        // re-sample alpha
        int f=factor*256.0f;
        int size=desc->pic_a[i]->w*desc->pic_a[i]->h;
        int j;
        mp_msg(MSGT_OSD, MSGL_DBG2, "font: resampling alpha by factor %5.3f (%d) ",factor,f);fflush(stdout);
        for(j=0;j<size;j++){
            int x=desc->pic_a[i]->bmp[j];	// alpha
            int y=desc->pic_b[i]->bmp[j];	// bitmap

#ifdef FAST_OSD
	    x=(x<(255-f))?0:1;
#else

	    x=255-((x*f)>>8); // scale
	    //if(x<0) x=0; else if(x>255) x=255;
	    //x^=255; // invert

	    if(x+y>255) x=255-y; // to avoid overflows

	    //x=0;
            //x=((x*f*(255-y))>>16);
            //x=((x*f*(255-y))>>16)+y;
            //x=(x*f)>>8;if(x<y) x=y;

            if(x<1) x=1; else
            if(x>=252) x=0;
#endif

            desc->pic_a[i]->bmp[j]=x;
//            desc->pic_b[i]->bmp[j]=0; // hack
        }
        mp_msg(MSGT_OSD, MSGL_DBG2, "DONE!\n");
    }
    if(!desc->height) desc->height=desc->pic_a[i]->h;
}

j='_';if(desc->font[j]<0) j='?';
for(i=0;i<65536;i++)
  if(desc->font[i]<0){
      desc->start[i]=desc->start[j];
      desc->width[i]=desc->width[j];
      desc->font[i]=desc->font[j];
  }
desc->font[' ']=-1;
desc->width[' ']=desc->spacewidth;

mp_msg(MSGT_OSD, MSGL_V, "Bitmap font %s loaded successfully! (%d chars)\n",fname,chardb);

return desc;

fail_out:
  if (f)
    fclose(f);
  if (desc) {
    free(desc->fpath);
    free(desc->name);
    free(desc);
  }
  return NULL;
}

#ifndef CONFIG_FREETYPE
void render_one_glyph(font_desc_t *desc, int c) {}
int kerning(font_desc_t *desc, int prevc, int c) { return 0; }
#endif
