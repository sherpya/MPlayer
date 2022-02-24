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

#define _BSD_SOURCE

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#include "osdep/glob.h"
#include "osdep/strsep.h"

#include "mp_msg.h"
#include "help_mp.h"
#include "stream/stream.h"
#include "mp_strings.h"

#include "mf.h"

int    mf_w = 0; //352; // let codecs to detect it
int    mf_h = 0; //288;
double mf_fps = 25.0;
char * mf_type = NULL; //"jpg";


static int init_mf_from_list_file(mf_t* mf, const char * filename){
   FILE *lst_f=fopen(filename + 1,"r");
   if ( lst_f )
    {
     char *fname=malloc(PATH_MAX);
     while ( fgets( fname,PATH_MAX,lst_f ) )
      {
       struct stat fs;
       /* remove spaces from end of fname */
       char *t=fname + strlen( fname ) - 1;
       while ( t > fname && isspace( *t ) ) *(t--)=0;
       if ( stat( fname,&fs ) )
        {
         mp_msg( MSGT_STREAM,MSGL_V,"[mf] file not found: '%s'\n",fname );
        }
        else
        {
         mf->names=realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
         mf->names[mf->nr_of_files]=strdup( fname );
         mf->nr_of_files++;
        }
      }
      fclose( lst_f );

      mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] number of files: %d\n",mf->nr_of_files );
      free( fname );
      return 1;
    }
    mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] %s is not indirect filelist\n",filename+1 );
    return 0;
}


static int init_mf_from_comma_delimited_paths(mf_t* mf, char * filename){
   char * fname;
   mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] filelist: %s\n",filename );

   while ( ( fname=strsep( &filename,"," ) ) )
    {
     struct stat fs;
     if ( stat( fname,&fs ) )
      {
       mp_msg( MSGT_STREAM,MSGL_V,"[mf] file not found: '%s'\n",fname );
      }
      else
      {
       mf->names=realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
       mf->names[mf->nr_of_files]=strdup( fname );
//       mp_msg( MSGT_STREAM,MSGL_V,"[mf] added file %d.: %s\n",mf->nr_of_files,mf->names[mf->nr_of_files] );
       mf->nr_of_files++;
      }
    }
   mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] number of files: %d\n",mf->nr_of_files );
   return 1;
}


static int init_mf_from_glob_pattern(mf_t* mf, const char * filename){
   glob_t gg;
   char *fname=malloc( strlen( filename ) + 32 );
   int i;

   strcpy( fname,filename );
   if ( !strchr( filename,'*' ) ) strcat( fname,"*" );

   mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] search expr: %s\n",fname );

   if ( glob( fname,0,NULL,&gg ) )
    { free( fname ); return 0; }

   mf->nr_of_files=gg.gl_pathc;
   mf->names=calloc( gg.gl_pathc, sizeof( char* ) );

   mp_msg( MSGT_STREAM, MSGL_INFO, "[mf] number of files: %d (%zu)\n", mf->nr_of_files, gg.gl_pathc * sizeof( char* ) );

   for( i=0;i < gg.gl_pathc;i++ )
    {
     struct stat fs;
     if (stat( gg.gl_pathv[i],&fs ) == -1) continue;
     if( S_ISDIR( fs.st_mode ) ) continue;
     mf->names[i]=strdup( gg.gl_pathv[i] );
//     mp_msg( MSGT_STREAM,MSGL_DBG2,"[mf] added file %d.: %s\n",i,mf->names[i] );
    }
   free( fname );
   globfree( &gg );
   return 1;
}


static int init_mf_from_printf_format(mf_t* mf, const char * filename){
 int count = 0, error_count = 0;
 mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] search expr: %s\n",filename );

 while ( error_count < 5 )
  {
   struct stat fs;
   char *fname = mp_asprintf( filename,count++ );

   if ( stat( fname,&fs ) )
    {
     error_count++;
     mp_msg( MSGT_STREAM,MSGL_V,"[mf] file not found: '%s'\n",fname );
     free(fname);
    }
    else
    {
     mf->names=realloc( mf->names,( mf->nr_of_files + 1 ) * sizeof( char* ) );
     mf->names[mf->nr_of_files]=fname;
//     mp_msg( MSGT_STREAM,MSGL_V,"[mf] added file %d.: %s\n",mf->nr_of_files,mf->names[mf->nr_of_files] );
     mf->nr_of_files++;
    }
  }

 mp_msg( MSGT_STREAM,MSGL_INFO,"[mf] number of files: %d\n",mf->nr_of_files );
 return 1;
}


mf_t* open_mf(char * filename){
#if defined(HAVE_GLOB) || defined(__MINGW32__)
 mf_t        * mf;
 int           init_success = 0;

 mf=calloc( 1,sizeof( mf_t ) );

 if( filename[0] == '@' )
  {
   init_success = init_mf_from_list_file(mf, filename);
  }

 if( !init_success )
  {
   if( strchr( filename,',') )
    {
     init_success = init_mf_from_comma_delimited_paths(mf, filename);
    }
   else if ( strchr( filename,'%' ) )
    {
     init_success = init_mf_from_printf_format(mf, filename);
    }
   else
    {
     init_success = init_mf_from_glob_pattern(mf, filename);
    }
  }

 if (!init_success)
  {
   free(mf);
   return NULL;
  }

 return mf;
#else
 mp_msg(MSGT_STREAM,MSGL_FATAL,"[mf] mf support is disabled on your os\n");
 return 0;
#endif
}
