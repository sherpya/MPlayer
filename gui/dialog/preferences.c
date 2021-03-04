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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "config.h"
#include "libavutil/avstring.h"
#include "help_mp.h"
#include "mixer.h"
#include "mpcommon.h"
#include "mplayer.h"
#include "libao2/audio_out.h"
#include "sub/sub.h"
#include "libvo/video_out.h"
#include "libvo/x11_common.h"
#include "stream/stream.h"
#include "libmpdemux/demuxer.h"
#include "libmpdemux/stheader.h"
#include "libmpcodecs/dec_video.h"
#include "sub/font_load.h"

#include "gui/app/app.h"
#include "gui/app/cfg.h"
#include "gui/app/gui.h"
#include "gui/interface.h"
#include "gui/ui/ui.h"
#include "dialog.h"
#include "gui/util/list.h"
#include "gui/util/mem.h"
#include "gui/util/string.h"
#include "gui/wm/ws.h"
#include "preferences.h"
#include "fileselect.h"
#include "tools.h"

/* for mpcodecs_[av]d_drivers: */
#include "libmpcodecs/vd.h"
#include "libmpcodecs/ad.h"

#ifdef CONFIG_ICONV
#include <iconv.h>
#endif

       GtkWidget * Preferences = NULL;
static GtkWidget * AConfig;
static GtkWidget * VConfig;
//static GtkWidget * BLoadSubtitle;
static GtkWidget * BLoadFont;
static GtkWidget * BOk;
static GtkWidget * BCancel;

static GtkWidget * CLADrivers;
static GtkWidget * CLVDrivers;

//static GtkWidget * ESubtitleName;
       GtkWidget * prEFontName;
       GtkWidget * prEDVDDevice;
       GtkWidget * prECDRomDevice;
static GtkWidget * EVFM;
static GtkWidget * EAFM;

static GtkWidget * CBVFM;
static GtkWidget * CBAFM;
static GtkWidget * CBAudioEqualizer;
//static GtkWidget * CBSurround;
static GtkWidget * CBExtraStereo;
static GtkWidget * CBReplayGain;
static GtkWidget * CBNormalize;
static GtkWidget * CBSoftwareMixer;
static GtkWidget * CBDoubleBuffer;
static GtkWidget * CBDR;
static GtkWidget * CBFramedrop;
static GtkWidget * CBHFramedrop;
//static GtkWidget * CBFullScreen;
static GtkWidget * CBShowVideoWindow;
static GtkWidget * CBNonInterlaved;
static GtkWidget * CBIndex;
static GtkWidget * CBFlip;
static GtkWidget * CBNoAutoSub;
static GtkWidget * CBSubUtf8;
static GtkWidget * CBSubUnicode;
static GtkWidget * CBSubOverlap;
static GtkWidget * CBDumpMPSub;
static GtkWidget * CBDumpSrt;
static GtkWidget * CBUseASS;
static GtkWidget * CBASSUseMargins;
static GtkWidget * SBASSTopMargin;
static GtkWidget * SBASSBottomMargin;
static GtkWidget * CBPostprocess;
static GtkWidget * CBCache;
static GtkWidget * CBLoadFullscreen;
static GtkWidget * CBSaveWinPos;
static GtkWidget * CBStopXScreenSaver;
static GtkWidget * CBPlayBar;
static GtkWidget * CBNoIdle;
static GtkWidget * CBTVDigital;
static GtkWidget * CBPlaylists;

static GtkWidget * SBCache;
static GtkAdjustment * SBCacheadj;

static GtkWidget * CBAutoSync;
static GtkWidget * SBAutoSync;
static GtkAdjustment * SBAutoSyncadj;

static GtkWidget * RBOSDNone;
static GtkWidget * RBOSDIndicator;
static GtkWidget * RBOSDTandP;
static GtkWidget * RBOSDTPTT;

static GtkWidget * HSAudioDelay;
static GtkWidget * HSExtraStereoMul;
static GtkWidget * HSPanscan;
static GtkWidget * HSSubDelay;
static GtkWidget * HSSubPosition;
static GtkWidget * HSSubFPS;
static GtkWidget * HSPPQuality;
static GtkWidget * HSFPS;

static GtkWidget *RGbox;
static GtkObject *RGadj;

static GtkAdjustment * HSExtraStereoMuladj, * HSAudioDelayadj, * HSPanscanadj, * HSSubDelayadj;
static GtkAdjustment * HSSubPositionadj, * HSSubFPSadj, * HSPPQualityadj, * HSFPSadj;

#ifdef CONFIG_FREETYPE
static GtkWidget     * HSFontBlur, * HSFontOutLine, * HSFontTextScale, * HSFontOSDScale;
static GtkAdjustment * HSFontBluradj, * HSFontOutLineadj, * HSFontTextScaleadj, * HSFontOSDScaleadj;
static GtkWidget     * CBFontEncoding, * EFontEncoding;
static GtkWidget     * RBFontNoAutoScale, * RBFontAutoScaleWidth, * RBFontAutoScaleHeight, * RBFontAutoScaleDiagonal;
//static GtkWidget     * AutoScale;
#else
static GtkWidget     * HSFontFactor;
static GtkAdjustment * HSFontFactoradj;
#endif

#ifdef CONFIG_ICONV
static GtkWidget     * CBSubEncoding, * ESubEncoding;
#endif

#if defined(CONFIG_FREETYPE) || defined(CONFIG_ICONV)
#define ENC_UTF8 "UTF-8"
#define ENC_UNICODE "UNICODE"
static struct
{
 char * name;
 char * comment;
} lEncoding[] =
 {
  { ENC_UTF8,      MSGTR_GUI_CpUTF8 },
  { "ISO-8859-1",  MSGTR_GUI_CpISO8859_1 },
  { "ISO-8859-2",  MSGTR_GUI_CpISO8859_2 },
  { "ISO-8859-3",  MSGTR_GUI_CpISO8859_3 },
  { "ISO-8859-4",  MSGTR_GUI_CpISO8859_4 },
  { "ISO-8859-5",  MSGTR_GUI_CpISO8859_5 },
  { "ISO-8859-6",  MSGTR_GUI_CpISO8859_6 },
  { "ISO-8859-7",  MSGTR_GUI_CpISO8859_7 },
  { "ISO-8859-8",  MSGTR_GUI_CpISO8859_8 },
  { "ISO-8859-9",  MSGTR_GUI_CpISO8859_9 },
  { "ISO-8859-10", MSGTR_GUI_CpISO8859_10 },
  { "ISO-8859-11", MSGTR_GUI_CpISO8859_11 },
  { "ISO-8859-13", MSGTR_GUI_CpISO8859_13 },
  { "ISO-8859-14", MSGTR_GUI_CpISO8859_14 },
  { "ISO-8859-15", MSGTR_GUI_CpISO8859_15 },
  { "ISO-8859-16", MSGTR_GUI_CpISO8859_16 },
  { "KOI8-R",      MSGTR_GUI_CpKOI8_R },
  { "KOI8-U",      MSGTR_GUI_CpKOI8_U },
  { "KOI8-RU",     MSGTR_GUI_CpKOI8_RU },
  { "BIG5",        MSGTR_GUI_CpBIG5 },
  { "SHIFT-JIS",   MSGTR_GUI_CpShiftJis },
  { "CP1250",      MSGTR_GUI_Cp1250},
  { "CP1251",      MSGTR_GUI_Cp1251},
  { "CP1252",      MSGTR_GUI_Cp1252 },
  { "CP1253",      MSGTR_GUI_Cp1253 },
  { "CP1254",      MSGTR_GUI_Cp1254 },
  { "CP1255",      MSGTR_GUI_Cp1255 },
  { "CP1256",      MSGTR_GUI_Cp1256 },
  { "CP1257",      MSGTR_GUI_Cp1257 },
  { "CP1258",      MSGTR_GUI_Cp1258 },
  { "CP874",       MSGTR_GUI_Cp874 },
  { "CP932",       MSGTR_GUI_Cp932 },
  { "CP936",       MSGTR_GUI_Cp936 },
  { "CP949",       MSGTR_GUI_Cp949 },
  { "CP950",       MSGTR_GUI_Cp950 },
  { ENC_UNICODE,   MSGTR_GUI_CpUnicode },
  { NULL,NULL }
 };
#endif

static int    old_audio_driver = 0;
static char * ao_driver[3];
static char * vo_driver[3];
static int    old_video_driver = 0;

static GtkWidget *AudioConfig;
static GtkWidget *DXR3Config;

 void ShowDXR3Config( void );
 void ShowAudioConfig( void );

static gboolean prHScaler( GtkWidget * widget,GdkEvent * event,gpointer user_data );
static void prToggled( GtkToggleButton * togglebutton,gpointer user_data );
static void prCListRow( GtkCList * clist,gint row,gint column,GdkEvent * event,gpointer user_data );

#if defined(CONFIG_FREETYPE) || defined(CONFIG_ICONV)
static void prEntry( GtkEditable * editable,gpointer user_data )
{
 const char * comment;
 int    i;

 (void) editable;

 switch( GPOINTER_TO_INT(user_data) )
  {
#ifdef CONFIG_FREETYPE
   case 0: // font encoding
        comment=gtk_entry_get_text( GTK_ENTRY( EFontEncoding ) );
        for ( i=0;lEncoding[i].name;i++ )
          if ( !gstrcmp( _(lEncoding[i].comment),comment ) ) break;
        if ( lEncoding[i].comment ) mplayer( MPLAYER_SET_FONT_ENCODING,0,lEncoding[i].name );
        else mplayer( MPLAYER_SET_FONT_ENCODING,0,(char *)comment );
        break;
#endif
#ifdef CONFIG_ICONV
   case 1: // sub encoding
        comment=gtk_entry_get_text( GTK_ENTRY( ESubEncoding ) );
        for ( i=0;lEncoding[i].name;i++ )
          if ( !gstrcmp( _(lEncoding[i].comment),comment ) ) break;
        if ( lEncoding[i].comment ) mplayer( MPLAYER_SET_SUB_ENCODING,0,lEncoding[i].name );
        else
        {
         if ( strcmp( comment,_(MSGTR_GUI__Default_) ) == 0 ) comment=NULL;
         mplayer( MPLAYER_SET_SUB_ENCODING,0,(char *)comment );
        }
        if ( !comment) gtk_widget_set_sensitive( CBSubEncoding,(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUtf8)) && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUnicode))) );
        gtk_widget_set_sensitive( CBSubUtf8,(comment == NULL) );
        gtk_widget_set_sensitive( CBSubUnicode,((comment == NULL) && !gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(CBSubUtf8) )) );
        break;
#endif
  }
}
#endif

#define bAConfig   0
#define bVconfig   1
#define bOk        2
#define bCancel    3
#define bLSubtitle 4
#define bLFont     5

static void button_toggled( GtkToggleButton *button, gpointer user_data )
{
 (void) button;
 (void) user_data;

#ifdef CONFIG_ICONV
 if ( !sub_cp ) gtk_widget_set_sensitive( CBSubEncoding,(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUtf8)) && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUnicode))) );
#endif
 gtk_widget_set_sensitive( CBSubUnicode,!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUtf8)) );
}

static void prButton( GtkButton * button, gpointer user_data )
{
 static int inform = True;

 (void) button;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case bOk:
        /* 1st page */
        gtkReplayGainOn = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBReplayGain));
        gtkReplayGainAdjustment = GTK_ADJUSTMENT(RGadj)->value;
        gtkEnableAudioEqualizer=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBAudioEqualizer ) );
        gtkAOExtraStereo=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBExtraStereo ) );
        gtkAONorm=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBNormalize ) );
        soft_vol=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBSoftwareMixer ) );
        mplayer( MPLAYER_SET_EXTRA_STEREO,HSExtraStereoMuladj->value,0 );
        audio_delay=HSAudioDelayadj->value;

        listSet( &audio_driver_list,ao_driver[0] );
        listSet( &video_driver_list,vo_driver[0] );

        /* 2nd page */
        vo_doublebuffering=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBDoubleBuffer ) );
        vo_directrendering=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBDR ) );

        frame_dropping=0;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBFramedrop ) ) == TRUE ) frame_dropping=1;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBHFramedrop ) ) == TRUE ) frame_dropping=2;

        flip=-1;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBFlip ) ) ) flip=1;

        force_fps=HSFPSadj->value;

        /* 3rd page */
        suboverlap_enabled=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBSubOverlap ) );
        sub_auto=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBNoAutoSub ) );
        gtkSubDumpMPSub=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBDumpMPSub ) );
        gtkSubDumpSrt=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBDumpSrt ) );
        sub_utf8=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBSubUtf8 ) );
        sub_unicode=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBSubUnicode ) );
#ifdef CONFIG_ASS
        gtkASS.enabled=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBUseASS ) );
        gtkASS.use_margins=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBASSUseMargins ) );
        gtkASS.top_margin=gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON( SBASSTopMargin ) );
        gtkASS.bottom_margin=gtk_spin_button_get_value_as_int( GTK_SPIN_BUTTON( SBASSBottomMargin ) );
#endif
        sub_delay=HSSubDelayadj->value;
        sub_fps=HSSubFPSadj->value;
        sub_pos=(int)HSSubPositionadj->value;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBOSDNone ) ) ) osd_level=0;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBOSDIndicator ) ) ) osd_level=1;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBOSDTandP ) ) ) osd_level=2;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBOSDTPTT ) ) ) osd_level=3;


        /* 4th page */
        setdup( &font_name,gtk_entry_get_text( GTK_ENTRY( prEFontName ) ) );
#ifdef CONFIG_FREETYPE
        mplayer( MPLAYER_SET_FONT_BLUR,HSFontBluradj->value,0 );
        mplayer( MPLAYER_SET_FONT_OUTLINE,HSFontOutLineadj->value,0 );
        mplayer( MPLAYER_SET_FONT_TEXTSCALE,HSFontTextScaleadj->value,0 );
        mplayer( MPLAYER_SET_FONT_OSDSCALE,HSFontOSDScaleadj->value,0 );
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBFontNoAutoScale ) ) ) mplayer( MPLAYER_SET_FONT_AUTOSCALE,0,0 );
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleHeight ) ) ) mplayer( MPLAYER_SET_FONT_AUTOSCALE,1,0 );
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleWidth ) ) ) mplayer( MPLAYER_SET_FONT_AUTOSCALE,2,0 );
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleDiagonal ) ) ) mplayer( MPLAYER_SET_FONT_AUTOSCALE,3,0 );
#else
        mplayer( MPLAYER_SET_FONT_FACTOR,HSFontFactoradj->value,0 );
#endif

        /* -- 5th page */
        force_ni=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBNonInterlaved ) );
        index_mode=-1;
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBIndex ) ) ) index_mode=1;

        {
         int i;
         const char * tmp = gtk_entry_get_text( GTK_ENTRY( EVFM ) );
         for( i=0;mpcodecs_vd_drivers[i];i++ )
          if ( !gstrcmp( tmp,mpcodecs_vd_drivers[i]->info->name ) )
           { listSet( &video_fm_list,mpcodecs_vd_drivers[i]->info->short_name ); break; }
        }

        {
         int i;
         const char * tmp = gtk_entry_get_text( GTK_ENTRY( EAFM ) );
         for( i=0;mpcodecs_ad_drivers[i];i++ )
          if ( !gstrcmp( tmp,mpcodecs_ad_drivers[i]->info->name ) )
           { listSet( &audio_fm_list,mpcodecs_ad_drivers[i]->info->short_name ); break; }
        }

        /* 6th page */
        gtkVfPP=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBPostprocess ) );
        fullscreen=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBLoadFullscreen ) );
        gui_save_pos=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBSaveWinPos ) );
        gtkShowVideoWindow=!gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBShowVideoWindow ) );
        stop_xscreensaver=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBStopXScreenSaver ) );
        gtkEnablePlayBar=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBPlayBar ) );
        gui_tv_digital=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBTVDigital ) );
        player_idle_mode=!gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBNoIdle ) );
        allow_playlist_parsing = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBPlaylists));
        mplayer( MPLAYER_SET_AUTO_QUALITY,HSPPQualityadj->value,0 );

        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBCache ) ) ) { gtkCacheSize=(int)SBCacheadj->value; gtkCacheOn=True; }
         else gtkCacheOn=False;

        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBAutoSync ) ) ) { gtkAutoSync=(int)SBAutoSyncadj->value; gtkAutoSyncOn=True; }
         else gtkAutoSyncOn=False;

        setdup( &dvd_device,gtk_entry_get_text( GTK_ENTRY( prEDVDDevice ) ) );
        setdup( &cdrom_device,gtk_entry_get_text( GTK_ENTRY( prECDRomDevice ) ) );

        if ( guiInfo.Playing && inform )
         {
          gtkMessageBox( MSGBOX_INFORMATION,_(MSGTR_GUI_MSG_PlaybackNeedsRestart) );
          inform = False;
         }

   case bCancel:
        gtk_widget_destroy( Preferences );
        if ( AudioConfig ) gtk_widget_destroy( AudioConfig );
#ifdef CONFIG_DXR3
        if ( DXR3Config ) gtk_widget_destroy( DXR3Config );
#endif
        break;
   case bAConfig:
        if ( !ao_driver[0] ) break;
        gtk_widget_set_sensitive( AConfig,FALSE );
        if ( !strncmp( ao_driver[0],"oss",3 ) ||
             !strncmp( ao_driver[0],"alsa",4 ) ||
             !strncmp( ao_driver[0],"esd",3 ) ||
             !strncmp( ao_driver[0],"sdl",3 ) ) {
          ShowAudioConfig();
          gtk_widget_set_sensitive( AConfig,TRUE );
        }
        break;
   case bVconfig:
        if ( !vo_driver[0] ) break;
        gtk_widget_set_sensitive( VConfig,FALSE );
#ifdef CONFIG_DXR3
        if ( !gstrcmp( vo_driver[0],"dxr3" ) ) { ShowDXR3Config(); gtk_widget_set_sensitive( VConfig,TRUE ); }
#endif
        break;
#if 0
   case bLSubtitle:
        break;
#endif
   case bLFont:
        ShowFileSelector( FILESELECT_FONT );
        gtkSetLayer( FileSelector );
        break;
  }
}

static gboolean prHScaler( GtkWidget * widget,GdkEvent * event,gpointer user_data )
{
 (void) widget;
 (void) event;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case 0: // extra stereo coefficient
        if ( !guiInfo.Playing ) break;
        mplayer( MPLAYER_SET_EXTRA_STEREO,HSExtraStereoMuladj->value,0 );
        break;
   case 1: // audio delay
        audio_delay=HSAudioDelayadj->value;
        break;
   case 2: // panscan
        mplayer( MPLAYER_SET_PANSCAN,HSPanscanadj->value,0 );
        break;
   case 3: // sub delay
        sub_delay=HSSubDelayadj->value;
        break;
   case 4: // sub position
        sub_pos=(int)HSSubPositionadj->value;
        break;
#ifdef CONFIG_FREETYPE
   case 6: // font blur
        mplayer( MPLAYER_SET_FONT_BLUR,HSFontBluradj->value,0 );
        break;
   case 7: // font outline
        mplayer( MPLAYER_SET_FONT_OUTLINE,HSFontOutLineadj->value,0 );
        break;
   case 8: // text scale
        mplayer( MPLAYER_SET_FONT_TEXTSCALE,HSFontTextScaleadj->value,0 );
        break;
   case 9: // osd scale
        mplayer( MPLAYER_SET_FONT_OSDSCALE,HSFontOSDScaleadj->value,0 );
        break;
#else
   case 5: // font factor
        mplayer( MPLAYER_SET_FONT_FACTOR,HSFontFactoradj->value,0 );
        break;
#endif
   case 10: // auto quality
        mplayer( MPLAYER_SET_AUTO_QUALITY,HSPPQualityadj->value,0 );
        break;
  }
 return FALSE;
}

static void prToggled( GtkToggleButton * togglebutton,gpointer user_data )
{
 int window;

 (void) togglebutton;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case 0: // extra stereo coefficient
        if ( guiInfo.Playing )
        gtk_widget_set_sensitive( HSExtraStereoMul,gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBExtraStereo ) ) );
        break;
//   case 1: // normalize
//   case 2: // equalizer
//      if ( guiInfo.Playing ) gtkMessageBox( MSGBOX_WARNING,"Please remember, this function need restart the playing." );
//      break;
   case 3:
        if ( !gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBShowVideoWindow ) ) )
         {
          window=wsShowWindow;
          gtkRaise( Preferences );
         } else window=wsHideWindow;
        // NOTE TO MYSELF: doesn't work with a fullscreen window
        if ( !guiInfo.Playing ) wsWindowVisibility( &guiApp.videoWindow,window );
        break;
   case 4:
   case 5:
   case 6:
   case 7:
        mplayer( MPLAYER_SET_FONT_AUTOSCALE,GPOINTER_TO_INT(user_data) - 4,0 );
        break;
   case 8:
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBCache ) ) ) gtk_widget_set_sensitive( SBCache,TRUE );
         else gtk_widget_set_sensitive( SBCache,FALSE );
        break;
   case 9:
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBAutoSync ) ) ) gtk_widget_set_sensitive( SBAutoSync,TRUE );
         else gtk_widget_set_sensitive( SBAutoSync,FALSE );
        break;
#ifdef CONFIG_ASS
   case 10:
        if ( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( CBUseASS ) ) )
         {
          gtk_widget_set_sensitive( CBASSUseMargins,TRUE );
          gtk_widget_set_sensitive( SBASSTopMargin,TRUE );
          gtk_widget_set_sensitive( SBASSBottomMargin,TRUE );
         }
        else
         {
          gtk_widget_set_sensitive( CBASSUseMargins,FALSE );
          gtk_widget_set_sensitive( SBASSTopMargin,FALSE );
          gtk_widget_set_sensitive( SBASSBottomMargin,FALSE );
         }
        break;
#endif
  case 11:
    gtk_widget_set_sensitive(RGbox, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBReplayGain)));
    break;
  }
}

static void prCListRow( GtkCList * clist,gint row,gint column,GdkEvent * event,gpointer user_data )
{
 (void) clist;
 (void) column;
 (void) event;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case 0: // audio driver
        gtk_clist_get_text( GTK_CLIST( CLADrivers ),row,0,(char **)&ao_driver );
        gtk_widget_set_sensitive( AConfig,FALSE );
        if ( !strncmp( ao_driver[0],"oss",3 ) ||
             !strncmp( ao_driver[0],"alsa",4 ) ||
             !strncmp( ao_driver[0],"esd",3 ) ||
             !strncmp( ao_driver[0],"sdl",3 ) )
          gtk_widget_set_sensitive( AConfig,TRUE );
        break;
   case 1: // video driver
        gtk_clist_get_text( GTK_CLIST( CLVDrivers ),row,0,(char **)&vo_driver );
        gtk_widget_set_sensitive( VConfig,FALSE );
#ifdef CONFIG_DXR3
        if ( !gstrcmp( vo_driver[0],"dxr3" ) ) gtk_widget_set_sensitive( VConfig,TRUE );
#endif
        break;
  }
}

static GtkWidget * CreatePreferences( void )
{
  GtkWidget * label;
  GtkWidget * frame;

  GtkWidget * vbox1;
  GtkWidget * notebook1;
  GtkWidget * hbox1;
  GtkWidget * vbox2;
  GtkWidget * scrolledwindow3;
  GtkWidget * vbox3;
  GtkWidget * hbox8;
  GtkWidget * hbox2;
  GtkWidget * hbox3;
  GtkWidget * vbox4;
  GtkWidget * scrolledwindow2;
  GtkWidget * hbuttonbox3;
  GtkWidget * vbox5;
  GtkWidget * vbox6;
  GtkWidget * vbox600;
  GSList    * OSD_group = NULL;
#ifdef CONFIG_FREETYPE
  GtkWidget * vbox11;
  GSList    * Font_group = NULL;
  GList     * CBFontEncoding_items = NULL;
#else
  GtkWidget * hbox7;
#endif
#ifdef CONFIG_ICONV
  iconv_t     cd;
  GList     * CBSubEncoding_items = NULL;
#endif
  GtkWidget * vbox7;
  GtkWidget * vbox8;
  GtkWidget * table1;
  GtkWidget * vbox9;
  GtkWidget * vbox10;
  GtkWidget * vbox603;
  GtkWidget * hbox6;
  GtkWidget * hbuttonbox5;
  GtkWidget * vbox601;
  GtkWidget * vbox602;
  GtkWidget * vbox604;
  GtkWidget * hbox5;
  GtkWidget * hbuttonbox1;
  GtkAccelGroup * accel_group;

  GtkWidget * hbox9;
  GtkWidget * hbox91;
  GtkWidget * hbox92;
  GtkWidget * hbox10;
  GtkWidget * RGspin;

  accel_group=gtk_accel_group_new();

  Preferences=gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW( Preferences ),_(MSGTR_GUI_Preferences) );
  gtk_window_set_position( GTK_WINDOW( Preferences ),GTK_WIN_POS_CENTER );
  gtk_window_set_wmclass( GTK_WINDOW( Preferences ),"Preferences",MPlayer );

  gtk_widget_realize( Preferences );
  gtkAddIcon( Preferences );

  vbox1=gtkAddVBox( gtkAddDialogFrame( Preferences ),0 );
  notebook1=gtk_notebook_new();
  gtk_widget_show( notebook1 );
  gtk_box_pack_start( GTK_BOX( vbox1 ),notebook1,TRUE,TRUE,0 );

  hbox1=gtkAddVBox( notebook1,0 );

  frame=gtkAddFrame( NULL,GTK_SHADOW_ETCHED_OUT,hbox1,1 );
  frame=gtkAddFrame( NULL,GTK_SHADOW_NONE,frame,1 );

/* 1st page */

  vbox2=gtkAddVBox( frame,0 );

  scrolledwindow3=gtk_scrolled_window_new( NULL,NULL );
  gtk_widget_show( scrolledwindow3 );
  gtk_box_pack_start( GTK_BOX( vbox2 ),scrolledwindow3,TRUE,TRUE,0 );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolledwindow3 ),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC );

  CLADrivers=gtk_clist_new( 2 );
  gtk_widget_show( CLADrivers );
  gtk_container_add( GTK_CONTAINER( scrolledwindow3 ),CLADrivers );
  gtk_clist_set_column_width( GTK_CLIST( CLADrivers ),0,65 );
  gtk_clist_column_titles_show( GTK_CLIST( CLADrivers ) );
  gtk_clist_set_shadow_type( GTK_CLIST( CLADrivers ),GTK_SHADOW_NONE );
  gtk_widget_set_usize( CLADrivers,250,-2 );
  gtk_clist_set_column_widget( GTK_CLIST( CLADrivers ),0,
    gtkAddLabel( _(MSGTR_GUI_AvailableDrivers),NULL ) );

  AConfig=gtkAddButton( _(MSGTR_GUI_ConfigureDriver),
    gtkAddHButtonBox( vbox2 ) );

  vbox3=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( NULL,GTK_SHADOW_ETCHED_OUT,hbox1,0 ),1 ),0 );
    gtk_widget_set_usize( vbox3,250,-2 );

  hbox10 = gtkAddHBox(vbox3, 1);
  CBReplayGain = gtkAddCheckButton(_(MSGTR_GUI_ReplayGain), hbox10);
  RGbox = gtkAddHBox(hbox10, 1);
  gtkAddLabel(_(MSGTR_GUI_ReplayGainAdjustment), RGbox);
  RGadj = gtk_adjustment_new(gtkReplayGainAdjustment, -30, 10, 1, 5, 0);
  RGspin = gtk_spin_button_new(GTK_ADJUSTMENT(RGadj), 1, 0);
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(RGspin), TRUE);
  gtk_box_pack_start(GTK_BOX(RGbox), GTK_WIDGET(RGspin), FALSE, FALSE, 0);
  gtkAddLabel(_(MSGTR_GUI_dB), RGbox);
  gtk_widget_set_sensitive(RGbox, gtkReplayGainOn);
  gtk_widget_show(RGspin);

  CBNormalize=gtkAddCheckButton( _(MSGTR_GUI_NormalizeSound),vbox3 );
  CBAudioEqualizer=gtkAddCheckButton( _(MSGTR_GUI_EnableEqualizer),vbox3 );
  CBSoftwareMixer=gtkAddCheckButton( _(MSGTR_GUI_EnableSoftwareMixer),vbox3 );
#if 0
  CBSurround=gtkAddCheckButton( "Enable surround",vbox3 );
#endif

  gtkAddHSeparator( vbox3 );
  CBExtraStereo=gtkAddCheckButton( _(MSGTR_GUI_EnableExtraStereo),vbox3 );
  hbox8=gtkAddHBox( vbox3,1 );
  /*label=*/gtkAddLabelColon( _(MSGTR_GUI_Coefficient),hbox8 );
//    gtk_misc_set_padding( GTK_MISC( label ),20,0 );
  HSExtraStereoMuladj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-10,10,0.1,0,0 ) );
  HSExtraStereoMul=gtkAddHScale( HSExtraStereoMuladj,hbox8,1 );
  gtkAddHSeparator( vbox3 );

  hbox8=gtkAddHBox( vbox3,1 );
  gtkAddLabelColon( _(MSGTR_GUI_AudioDelay),hbox8 );

  HSAudioDelayadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-100,100,0.1,0,0 ) );
  HSAudioDelay=gtkAddHScale( HSAudioDelayadj,hbox8,1 );
  label=gtkAddLabel( _(MSGTR_GUI_Audio),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),0 ),label );

/* 2nd page */

  hbox2=gtkAddVBox( notebook1,0 );

  vbox4=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( NULL,GTK_SHADOW_ETCHED_OUT,hbox2,1 ),1 ),0 );

  scrolledwindow2=gtk_scrolled_window_new( NULL,NULL );
  gtk_widget_show( scrolledwindow2 );
  gtk_box_pack_start( GTK_BOX( vbox4 ),scrolledwindow2,TRUE,TRUE,0 );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolledwindow2 ),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC );

  CLVDrivers=gtk_clist_new( 2 );
  gtk_widget_show( CLVDrivers );
  gtk_container_add( GTK_CONTAINER( scrolledwindow2 ),CLVDrivers );
  gtk_clist_set_column_width( GTK_CLIST( CLVDrivers ),0,65 );
  gtk_clist_column_titles_show( GTK_CLIST( CLVDrivers ) );
  gtk_clist_set_shadow_type( GTK_CLIST( CLVDrivers ),GTK_SHADOW_NONE );
  gtk_widget_set_usize( CLVDrivers,250,-2 );

  label=gtkAddLabel( _(MSGTR_GUI_AvailableDrivers),NULL );
    gtk_clist_set_column_widget( GTK_CLIST( CLVDrivers ),0,label );

  hbuttonbox3=gtkAddHButtonBox( vbox4 );
  VConfig=gtkAddButton( _(MSGTR_GUI_ConfigureDriver),hbuttonbox3 );

  vbox5=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( NULL,GTK_SHADOW_ETCHED_OUT,hbox2,0 ),1 ),0 );
    gtk_widget_set_usize( vbox5,250,-2 );

  CBDoubleBuffer=gtkAddCheckButton( _(MSGTR_GUI_EnableDoubleBuffering),vbox5 );
  CBDR=gtkAddCheckButton( _(MSGTR_GUI_EnableDirectRendering),vbox5 );
  CBFramedrop=gtkAddCheckButton( _(MSGTR_GUI_EnableFrameDropping),vbox5 );
  CBHFramedrop=gtkAddCheckButton( _(MSGTR_GUI_EnableFrameDroppingIntense),vbox5 );
  CBFlip=gtkAddCheckButton( _(MSGTR_GUI_Flip),vbox5 );

  table1=gtk_table_new( 2,2,FALSE );
  gtk_widget_show( table1 );
  gtk_box_pack_start( GTK_BOX( vbox5 ),table1,FALSE,FALSE,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_PanAndScan),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_FrameRate),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSPanscanadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,1,0.1,0,0 ) );
  HSPanscan=gtkAddHScale( HSPanscanadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSPanscan,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSFPSadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,120,0.001,0,0 ) );
  HSFPS=gtk_spin_button_new( GTK_ADJUSTMENT( HSFPSadj ),1,3 );
    gtk_widget_show( HSFPS );
    gtk_spin_button_set_numeric( GTK_SPIN_BUTTON( HSFPS ),TRUE );
    gtk_table_attach( GTK_TABLE( table1 ),HSFPS,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabel( _(MSGTR_GUI_Video),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),1 ),label );

/* 3rd page */

  vbox6=gtkAddVBox( notebook1,0 );

  vbox600=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_OsdLevel),GTK_SHADOW_ETCHED_OUT,vbox6,0 ),1 ),0 );

  RBOSDNone=gtkAddRadioButton( _(MSGTR_GUI_OsdLevel0),&OSD_group,vbox600 );
  RBOSDIndicator=gtkAddRadioButton( _(MSGTR_GUI_OsdLevel1),&OSD_group,vbox600 );
  RBOSDTandP=gtkAddRadioButton( _(MSGTR_GUI_OsdLevel2),&OSD_group,vbox600 );
  RBOSDTPTT=gtkAddRadioButton( _(MSGTR_GUI_OsdLevel3),&OSD_group,vbox600 );

  vbox7=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_Subtitle),GTK_SHADOW_ETCHED_OUT,vbox6,0 ),1 ),0 );

#if 0
  hbox4=gtkAddHBox( vbox7,1 );

  gtkAddLabelColon( _(MSGTR_GUI_Subtitle),hbox4 );

  ESubtitleName=gtk_entry_new();
  gtk_widget_show( ESubtitleName );
  gtk_box_pack_start( GTK_BOX( hbox4 ),ESubtitleName,TRUE,TRUE,0 );

  hbuttonbox4=gtkAddHButtonBox( hbuttonbox4 );
    gtk_container_set_border_width( GTK_CONTAINER( hbuttonbox4 ),3 );
  BLoadSubtitle=gtkAddButton( _(MSGTR_GUI_Browse),hbuttonbox4 );
#endif

  vbox8=gtkAddVBox( vbox7,0 );

  table1=gtk_table_new( 4,2,FALSE );
  gtk_widget_show( table1 );
  gtk_box_pack_start( GTK_BOX( vbox8 ),table1,FALSE,FALSE,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_Delay),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_Position),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_FrameRate),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,2,3,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_Encoding),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,3,4,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_FILL ),0,0 );

  HSSubDelayadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-10.0,10,0.1,0,0 ) );
  HSSubDelay=gtkAddHScale( HSSubDelayadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSSubDelay,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSSubPositionadj=GTK_ADJUSTMENT( gtk_adjustment_new( 100,0,150,1,0,0 ) );
  HSSubPosition=gtkAddHScale( HSSubPositionadj,NULL,0 );
    gtk_table_attach( GTK_TABLE( table1 ),HSSubPosition,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSSubFPSadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,120,0.001,0,0 ) );
  HSSubFPS=gtk_spin_button_new( GTK_ADJUSTMENT( HSSubFPSadj ),1,3 );
    gtk_widget_show( HSSubFPS );
    gtk_widget_set_usize( HSSubFPS,60,-1 );
    gtk_spin_button_set_numeric( GTK_SPIN_BUTTON( HSSubFPS ),TRUE );
    gtk_table_attach( GTK_TABLE( table1 ),HSSubFPS,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  vbox10=gtkAddVBox( NULL,0 );
  gtk_table_attach( GTK_TABLE( table1 ),vbox10,1,2,3,4,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

#ifdef CONFIG_ICONV
  gtk_misc_set_alignment( GTK_MISC( label ),0,0 );
  CBSubEncoding=gtk_combo_new();
  gtk_widget_show( CBSubEncoding );
  gtk_box_pack_start( GTK_BOX( vbox10 ),CBSubEncoding,TRUE,FALSE,0 );
  CBSubEncoding_items=g_list_append( CBSubEncoding_items,_(MSGTR_GUI__Default_) );
  {
   int i, listed=(sub_cp == NULL);

   for ( i=0;lEncoding[i].name;i++ )
   {
    cd=iconv_open( ENC_UTF8,lEncoding[i].name );

    if (cd != (iconv_t) -1)
    {
     iconv_close(cd);
     CBSubEncoding_items=g_list_append( CBSubEncoding_items,_(lEncoding[i].comment) );

     if ( !listed )
      if ( av_strcasecmp ( lEncoding[i].name, sub_cp ) == 0 ) listed=True;
    }
   }
   if ( !listed ) CBSubEncoding_items=g_list_insert( CBSubEncoding_items,sub_cp,1 );
  }
  gtk_combo_set_popdown_strings( GTK_COMBO( CBSubEncoding ),CBSubEncoding_items );
  g_list_free( CBSubEncoding_items );

  ESubEncoding=GTK_COMBO( CBSubEncoding )->entry;
  gtk_entry_set_editable( GTK_ENTRY( ESubEncoding ),FALSE );
  gtk_widget_show( ESubEncoding );
#endif

  hbox3=gtkAddHBox( NULL,0 );
  gtk_box_set_spacing( GTK_BOX( hbox3 ), 24 );
  gtk_box_pack_start( GTK_BOX( vbox10 ),hbox3,TRUE,FALSE,0 );

  vbox9=gtkAddVBox( vbox8,0 );

  CBSubOverlap=gtkAddCheckButton( _(MSGTR_GUI_SubtitleAllowOverlap),vbox9 );
  CBNoAutoSub=gtkAddCheckButton( _(MSGTR_GUI_SubtitleAutomaticLoad),vbox9 );
  CBSubUtf8=gtkAddCheckButton( _(MSGTR_GUI_CpUTF8),hbox3 );
  CBSubUnicode=gtkAddCheckButton( _(MSGTR_GUI_CpUnicode),hbox3 );
  CBDumpMPSub=gtkAddCheckButton( _(MSGTR_GUI_SubtitleConvertMpsub),vbox9 );
  CBDumpSrt=gtkAddCheckButton( _(MSGTR_GUI_SubtitleConvertSrt),vbox9 );

  gtkAddHSeparator( vbox9 );
  CBUseASS=gtkAddCheckButton( _(MSGTR_GUI_EnableAssSubtitle),vbox9 );
  hbox9=gtkAddHBox( vbox9,0 );
  CBASSUseMargins=gtkAddCheckButton( _(MSGTR_GUI_SubtitleAddMargins),hbox9 );

  hbox91=gtk_hbox_new( FALSE,0 );
  gtk_box_pack_start( GTK_BOX( hbox9 ),hbox91,TRUE,FALSE,0 );
  gtk_widget_show( hbox91 );
  SBASSTopMargin=gtkAddSpinButtonColon( _(MSGTR_GUI_Top),
        (GtkAdjustment *)gtk_adjustment_new(0,0,2000,1,8,0),hbox91 );

  hbox92=gtk_hbox_new( FALSE,0 );
  gtk_box_pack_start( GTK_BOX( hbox9 ),hbox92,TRUE,FALSE,0 );
  gtk_widget_show( hbox92 );
  SBASSBottomMargin=gtkAddSpinButtonColon( _(MSGTR_GUI_Bottom),
        (GtkAdjustment *)gtk_adjustment_new(0,0,2000,1,8,0),hbox92 );

  label=gtkAddLabel( _(MSGTR_GUI_OSD_Subtitles),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),2 ),label );
  vbox601=gtkAddVBox( notebook1,0 );

/* 4th page */

  vbox603=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( NULL,GTK_SHADOW_ETCHED_OUT,vbox601,0 ),1 ),0 );

  hbox6=gtkAddHBox( vbox603,1 );
  gtkAddLabelColon( _(MSGTR_GUI_Font),hbox6 );
  prEFontName=gtk_entry_new();
  gtk_widget_show( prEFontName );
  gtk_box_pack_start( GTK_BOX( hbox6 ),prEFontName,TRUE,TRUE,0 );
  hbuttonbox5=gtkAddHButtonBox( hbox6 );
    gtk_container_set_border_width( GTK_CONTAINER( hbuttonbox5 ),3 );
  BLoadFont=gtkAddButton( _(MSGTR_GUI_Browse),hbuttonbox5 );

#ifdef CONFIG_FREETYPE
  table1=gtk_table_new( 6,2,FALSE );
  gtk_widget_show( table1 );
  gtk_box_pack_start( GTK_BOX( vbox603 ),table1,FALSE,FALSE,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_ScaleMode),NULL );
  gtk_misc_set_alignment( GTK_MISC( label ),0,0 );
  gtk_table_attach( GTK_TABLE( table1 ),label,0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_FILL ),0,0 );

  vbox11=gtkAddVBox( NULL,0 );
  gtk_table_attach( GTK_TABLE( table1 ),vbox11,1,2,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  RBFontNoAutoScale=gtkAddRadioButton( _(MSGTR_GUI_ScaleNo),&Font_group,vbox11 );
  RBFontAutoScaleHeight=gtkAddRadioButton( _(MSGTR_GUI_ScaleMovieHeight),&Font_group,vbox11 );
  RBFontAutoScaleWidth=gtkAddRadioButton( _(MSGTR_GUI_ScaleMovieWidth),&Font_group,vbox11 );
  RBFontAutoScaleDiagonal=gtkAddRadioButton( _(MSGTR_GUI_ScaleMovieDiagonal),&Font_group,vbox11 );

  label=gtkAddLabelColon( _(MSGTR_GUI_Encoding),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CBFontEncoding=gtk_combo_new();
  gtk_widget_show( CBFontEncoding );
  gtk_table_attach( GTK_TABLE( table1 ),CBFontEncoding,1,2,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  {
   int i, append, listed=(subtitle_font_encoding == NULL);
   for ( i=0;lEncoding[i].name;i++ )
   {
    append=(av_strcasecmp( lEncoding[i].name,ENC_UNICODE ) == 0);
#ifdef CONFIG_ICONV
    cd=iconv_open( "ucs-4",lEncoding[i].name );

    if (cd != (iconv_t) -1)
    {
     iconv_close(cd);
     append=True;
    }
#endif
    if ( append )
    {
     CBFontEncoding_items=g_list_append( CBFontEncoding_items,_(lEncoding[i].comment) );

     if ( !listed )
      if ( av_strcasecmp ( lEncoding[i].name, subtitle_font_encoding ) == 0 ) listed=True;
    }
   }
   if ( !listed ) CBFontEncoding_items=g_list_insert( CBFontEncoding_items,subtitle_font_encoding,1 );
  }
  gtk_combo_set_popdown_strings( GTK_COMBO( CBFontEncoding ),CBFontEncoding_items );
  g_list_free( CBFontEncoding_items );

  EFontEncoding=GTK_COMBO( CBFontEncoding )->entry;
  gtk_entry_set_editable( GTK_ENTRY( EFontEncoding ),FALSE );
  gtk_widget_show( EFontEncoding );

  label=gtkAddLabelColon( _(MSGTR_GUI_Blur),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,2,3,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSFontBluradj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,100,0.1,0,0 ) );
  HSFontBlur=gtkAddHScale( HSFontBluradj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSFontBlur,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_Outline),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,3,4,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSFontOutLineadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,100,0.1,0,0 ) );
  HSFontOutLine=gtkAddHScale( HSFontOutLineadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSFontOutLine,1,2,3,4,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_SizeSubtitles),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,4,5,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSFontTextScaleadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,100,0.1,0,0 ) );
  HSFontTextScale=gtkAddHScale( HSFontTextScaleadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSFontTextScale,1,2,4,5,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_SizeOSD),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,5,6,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  HSFontOSDScaleadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,100,0.1,0,0 ) );
  HSFontOSDScale=gtkAddHScale( HSFontOSDScaleadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),HSFontOSDScale,1,2,5,6,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
#else
  hbox7=gtkAddHBox( vbox603,1 );
  gtkAddLabelColon( _(MSGTR_GUI_Outline),hbox7 );
  HSFontFactoradj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,10,0.05,0,0 ) );
  HSFontFactor=gtkAddHScale( HSFontFactoradj,hbox7,2 );
#endif

  label=gtkAddLabel( _(MSGTR_GUI_Font),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),3 ),label );

/* 5th page */

  vbox601=gtkAddVBox( notebook1,0 );

  vbox602=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_Demuxer),GTK_SHADOW_ETCHED_OUT,vbox601,0 ),1 ),0 );

  CBNonInterlaved=gtkAddCheckButton( _(MSGTR_GUI_NonInterleavedParser),vbox602 );
  CBIndex=gtkAddCheckButton( _(MSGTR_GUI_SeekingInBrokenMedia),vbox602 );

  vbox604=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_Codecs),GTK_SHADOW_ETCHED_OUT,vbox601,0 ),1 ),0 );

  hbox5=gtkAddHBox( vbox604,1 );

  gtkAddLabelColon( _(MSGTR_GUI_CodecFamilyVideo),hbox5 );

  CBVFM=gtk_combo_new();
  gtk_widget_show( CBVFM );
  gtk_box_pack_start( GTK_BOX( hbox5 ),CBVFM,TRUE,TRUE,0 );

  EVFM=GTK_COMBO( CBVFM )->entry;
  gtk_entry_set_editable( GTK_ENTRY( EVFM ),FALSE );
  gtk_widget_show( EVFM );

  hbox5=gtkAddHBox( vbox604,1 );

  gtkAddLabelColon( _(MSGTR_GUI_CodecFamilyAudio),hbox5 );

  CBAFM=gtk_combo_new();
  gtk_widget_show( CBAFM );
  gtk_box_pack_start( GTK_BOX( hbox5 ),CBAFM,TRUE,TRUE,0 );

  EAFM=GTK_COMBO( CBAFM )->entry;
  gtk_entry_set_editable( GTK_ENTRY( EAFM ),FALSE );
  gtk_widget_show( EAFM );

  label=gtkAddLabel( _(MSGTR_GUI_Demuxers_Codecs),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),4 ),label );

  vbox601=gtkAddVBox( notebook1,0 );

/* 6th page */

  vbox602=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_PostProcessing),GTK_SHADOW_ETCHED_OUT,vbox601,0 ),1 ),0 );

  CBPostprocess=gtkAddCheckButton( _(MSGTR_GUI_EnablePostProcessing),vbox602 );

  hbox5=gtkAddHBox( vbox602,1 );

  gtkAddLabelColon( _(MSGTR_GUI_MaximumUsageSpareCPU),hbox5 );

  if ( guiInfo.sh_video && guiInfo.Playing ) HSPPQualityadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,get_video_quality_max( guiInfo.sh_video ),1,0,0 ) );
   else HSPPQualityadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,100,1,0,0 ) );
  HSPPQuality=gtkAddHScale( HSPPQualityadj,hbox5,0 );

  vbox602=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_Cache),GTK_SHADOW_ETCHED_OUT,vbox601,0 ),1 ),0 );

  CBCache=gtkAddCheckButton( _(MSGTR_GUI_EnableCache),vbox602 );

  hbox5=gtkAddHBox( vbox602,1 );

  gtkAddLabelColon( _(MSGTR_GUI_CacheSize),hbox5 );

  SBCacheadj=GTK_ADJUSTMENT( gtk_adjustment_new( gtkCacheSize,32,0x7fffffff,1,32,0 ) );
  SBCache=gtk_spin_button_new( GTK_ADJUSTMENT( SBCacheadj ),1,0 );
  gtk_widget_show( SBCache );
  gtk_box_pack_start( GTK_BOX( hbox5 ),SBCache,TRUE,TRUE,0 );

  vbox602=gtkAddVBox(
    gtkAddFrame( NULL,GTK_SHADOW_NONE,
      gtkAddFrame( _(MSGTR_GUI_Miscellaneous),GTK_SHADOW_ETCHED_OUT,vbox601,1 ),1 ),0 );

  CBShowVideoWindow=gtkAddCheckButton( _(MSGTR_GUI_HideVideoWindow),vbox602 );
  CBLoadFullscreen=gtkAddCheckButton( _(MSGTR_GUI_StartFullscreen),vbox602 );
  CBSaveWinPos=gtkAddCheckButton( _(MSGTR_GUI_SaveWindowPositions),vbox602 );
  CBStopXScreenSaver=gtkAddCheckButton( _(MSGTR_GUI_TurnOffXScreenSaver),vbox602 );
  CBPlayBar=gtkAddCheckButton( _(MSGTR_GUI_EnablePlaybar),vbox602 );
  CBTVDigital=gtkAddCheckButton( _(MSGTR_GUI_EnableDigitalTV),vbox602 );
  CBPlaylists = gtkAddCheckButton(_(MSGTR_GUI_PlaylistSupport), vbox602);
  CBNoIdle=gtkAddCheckButton( _(MSGTR_GUI_QuitAfterPlaying),vbox602 );

  gtkAddHSeparator( vbox602 );

  CBAutoSync=gtkAddCheckButton( _(MSGTR_GUI_EnableAutomaticAVSync),vbox602 );
  hbox5=gtkAddHBox( vbox602,1 );
  gtkAddLabelColon( _(MSGTR_GUI_SyncValue),hbox5 );
  SBAutoSyncadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,0,10000,1,10,0 ) );
  SBAutoSync=gtk_spin_button_new( GTK_ADJUSTMENT( SBAutoSyncadj ),1,0 );
  gtk_widget_show( SBAutoSync );
  gtk_box_pack_start( GTK_BOX( hbox5 ),SBAutoSync,TRUE,TRUE,0 );

  gtkAddHSeparator( vbox602 );

  table1=gtk_table_new( 2,2,FALSE );
    gtk_widget_show( table1 );
    gtk_box_pack_start( GTK_BOX( vbox602 ),table1,FALSE,FALSE,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_DeviceDVD),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  prEDVDDevice=gtk_entry_new();
    gtk_widget_show( prEDVDDevice );
    gtk_table_attach( GTK_TABLE( table1 ),prEDVDDevice,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  label=gtkAddLabelColon( _(MSGTR_GUI_DeviceCDROM),NULL );
    gtk_table_attach( GTK_TABLE( table1 ),label,0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  prECDRomDevice=gtk_entry_new();
    gtk_widget_show( prECDRomDevice );
    gtk_table_attach( GTK_TABLE( table1 ),prECDRomDevice,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

//  gtkAddHSeparator( vbox602 );

  label=gtkAddLabel( _(MSGTR_GUI_Miscellaneous),NULL );
    gtk_notebook_set_tab_label( GTK_NOTEBOOK( notebook1 ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook1 ),5 ),label );

/* --- */

  gtkAddHSeparator( vbox1 );

  hbuttonbox1=gtkAddHButtonBox( vbox1 );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
    gtk_button_box_set_spacing( GTK_BUTTON_BOX( hbuttonbox1 ),10 );
  BOk=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );
  BCancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox1 );

  gtk_widget_add_accelerator( BOk,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );
  gtk_widget_add_accelerator( BCancel,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );

  gtk_signal_connect( GTK_OBJECT( Preferences ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&Preferences );

  gtk_signal_connect( GTK_OBJECT( AConfig ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bAConfig) );
  gtk_signal_connect( GTK_OBJECT( BOk ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bOk) );
  gtk_signal_connect( GTK_OBJECT( BCancel ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bCancel) );
  gtk_signal_connect( GTK_OBJECT( VConfig ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bVconfig) );
#if 0
  gtk_signal_connect( GTK_OBJECT( BLoadSubtitle ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bLSubtitle) );
#endif
  gtk_signal_connect( GTK_OBJECT( BLoadFont ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(bLFont) );

  gtk_signal_connect( GTK_OBJECT( CBSubUtf8 ),"toggled",GTK_SIGNAL_FUNC( button_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBSubUnicode ),"toggled",GTK_SIGNAL_FUNC( button_toggled ),NULL );
#if 0
  gtk_signal_connect( GTK_OBJECT( CBNormalize ),"toggled",GTK_SIGNAL_FUNC( on_CBNormalize_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBSoftwareMixer ),"toggled",GTK_SIGNAL_FUNC( on_CBSoftwareMixer_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBSurround ),"toggled",GTK_SIGNAL_FUNC( on_CBSurround_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBExtraStereo ),"toggled",GTK_SIGNAL_FUNC( on_CBExtraStereo_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBDoubleBuffer ),"toggled",GTK_SIGNAL_FUNC( on_CBDoubleBuffer_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBDR ),"toggled",GTK_SIGNAL_FUNC( on_CBDR_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBFramedrop ),"toggled",GTK_SIGNAL_FUNC( on_CBFramedrop_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBHFramedrop ),"toggled",GTK_SIGNAL_FUNC( on_CBHFramedrop_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBFullScreen ),"toggled",GTK_SIGNAL_FUNC( on_CBFullScreen_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBNonInterlaved ),"toggled",GTK_SIGNAL_FUNC( on_CBNonInterlaved_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBFlip ),"toggled",GTK_SIGNAL_FUNC( on_CBFlip_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBPostprocess ),"toggled",GTK_SIGNAL_FUNC( on_CBPostprocess_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBNoAutoSub ),"toggled",GTK_SIGNAL_FUNC( on_CBNoAutoSub_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBDumpMPSub ),"toggled",GTK_SIGNAL_FUNC( on_CBDumpMPSub_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBDumpSrt ),"toggled",GTK_SIGNAL_FUNC( on_CBDumpSrt_toggled ),NULL );
#endif
#if 0
  gtk_signal_connect( GTK_OBJECT( RBOSDNone ),"toggled",GTK_SIGNAL_FUNC( on_RBOSDNone_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( RBOSDIndicator ),"toggled",GTK_SIGNAL_FUNC( on_RBOSDIndicator_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( RBOSDTandP ),"toggled",GTK_SIGNAL_FUNC( on_RBOSDTandP_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( RBOSDTPTT ),"toggled",GTK_SIGNAL_FUNC( on_RBOSDIndicator_toggled ),NULL );
  gtk_signal_connect( GTK_OBJECT( CBAudioEqualizer ),"toggled",GTK_SIGNAL_FUNC( on_CBAudioEqualizer_toggled ),NULL );
#endif
#if 0
  gtk_signal_connect( GTK_OBJECT( HSAudioDelay ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSAudioDelay_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSPanscan ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSPanscan_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( label2 ),"motion-notify-event",GTK_SIGNAL_FUNC( on_label2_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSSubDelay ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSSubDelay_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSSubPosition ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSSubPosition_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSSubFPS ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSSubFPS_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSFontFactor ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSFontFactor_motion_notify_event ),NULL );
  gtk_signal_connect( GTK_OBJECT( HSPPQuality ),"motion-notify-event",GTK_SIGNAL_FUNC( on_HSPPQuality_motion_notify_event ),NULL );
#endif

  gtk_notebook_set_page( GTK_NOTEBOOK( notebook1 ),2 );

  gtk_window_add_accel_group( GTK_WINDOW( Preferences ),accel_group );

  return Preferences;
}

void ShowPreferences( void )
{
 if ( Preferences ) gtkRaise( Preferences );
   else Preferences=CreatePreferences();

/* 1st page */
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBAudioEqualizer ),gtkEnableAudioEqualizer );
#if 0
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSurround ),gtkAOSurround );
#endif
 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CBReplayGain), gtkReplayGainOn);
 gtk_adjustment_set_value(GTK_ADJUSTMENT(RGadj), gtkReplayGainAdjustment);
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBExtraStereo ),gtkAOExtraStereo );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBNormalize ),gtkAONorm );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSoftwareMixer ),soft_vol );
 gtk_adjustment_set_value( HSExtraStereoMuladj,gtkAOExtraStereoMul );
 {
  int    i = 0;
  char * tmp[3]; tmp[2]="";
  old_audio_driver=-1;
  if ( CLADrivers ) gtk_clist_clear( GTK_CLIST( CLADrivers ) );
  while ( audio_out_drivers[i] )
   {
    const ao_info_t *info = audio_out_drivers[i++]->info;
    if ( !strcmp( info->short_name,"mpegpes" ) ) continue;
    if ( !strcmp( info->short_name,"pcm" ) ) continue;
    if ( !strcmp( info->short_name,"plugin" ) ) continue;
    if ( audio_driver_list )
     {
      char * name = gstrdup( audio_driver_list[0] );
      char * sep = gstrchr( audio_driver_list[0],':' );
      if ( sep ) *sep=0;
      if ( !gstrcmp( name,info->short_name ) ) old_audio_driver=i - 1;
      free( name );
     }
    tmp[0]=(char *)info->short_name; tmp[1]=(char *)info->name; gtk_clist_append( GTK_CLIST( CLADrivers ),tmp );
   }
  if ( old_audio_driver > -1 )
   {
    gtk_clist_select_row( GTK_CLIST( CLADrivers ),old_audio_driver,0 );
    gtk_clist_get_text( GTK_CLIST( CLADrivers ),old_audio_driver,0,(char **)&ao_driver );
    gtk_widget_set_sensitive( AConfig,FALSE );
    if ( !strncmp( ao_driver[0],"oss",3 ) ||
         !strncmp( ao_driver[0],"alsa",4 ) ||
         !strncmp( ao_driver[0],"esd",3 ) ||
         !strncmp( ao_driver[0],"sdl",3 ) )
      gtk_widget_set_sensitive( AConfig,TRUE );
   }
 }

/* 2nd page */
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBDoubleBuffer ),vo_doublebuffering );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBDR ),vo_directrendering );

 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBFramedrop ),FALSE );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBHFramedrop ),FALSE );
 switch ( frame_dropping )
  {
   case 2: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBHFramedrop ),TRUE );
   case 1: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBFramedrop ),TRUE );
  }

 if (flip != -1)
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBFlip ),flip );
 gtk_adjustment_set_value( HSPanscanadj,vo_panscan );

 {
  int i = 0, c = 0;
  char * tmp[3]; tmp[2]="";
  old_video_driver=0;
  if ( CLVDrivers ) gtk_clist_clear( GTK_CLIST( CLVDrivers ) );
  while ( video_out_drivers[i] )
   if ( video_out_drivers[i++]->control( VOCTRL_GUISUPPORT,NULL ) == VO_TRUE )
    {
     if ( video_driver_list && !gstrcmp( video_driver_list[0],video_out_drivers[i - 1]->info->short_name ) ) old_video_driver=c; c++;
     tmp[0]=(char *)video_out_drivers[i - 1]->info->short_name; tmp[1]=(char *)video_out_drivers[i - 1]->info->name;
     gtk_clist_append( GTK_CLIST( CLVDrivers ),tmp );
    }
  gtk_clist_select_row( GTK_CLIST( CLVDrivers ),old_video_driver,0 );
  gtk_clist_get_text( GTK_CLIST( CLVDrivers ),old_video_driver,0,(char **)&vo_driver );
  gtk_widget_set_sensitive( VConfig,FALSE );
#ifdef CONFIG_DXR3
  if ( !gstrcmp( vo_driver[0],"dxr3" ) ) gtk_widget_set_sensitive( VConfig,TRUE );
#endif
 }

  gtk_adjustment_set_value( HSFPSadj,force_fps );

/* 3rd page */
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSubOverlap ),suboverlap_enabled );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBNoAutoSub ),sub_auto );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBDumpMPSub ),gtkSubDumpMPSub );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBDumpSrt ),gtkSubDumpSrt );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSubUtf8 ),sub_utf8 );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSubUnicode ),sub_unicode );
#ifdef CONFIG_ASS
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBUseASS ),gtkASS.enabled );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBASSUseMargins ),gtkASS.use_margins );
 gtk_spin_button_set_value( (GtkSpinButton *)SBASSTopMargin,gtkASS.top_margin );
 gtk_spin_button_set_value( (GtkSpinButton *)SBASSBottomMargin,gtkASS.bottom_margin );

 if ( !gtkASS.enabled )
  {
   gtk_widget_set_sensitive( CBASSUseMargins,FALSE );
   gtk_widget_set_sensitive( SBASSTopMargin,FALSE );
   gtk_widget_set_sensitive( SBASSBottomMargin,FALSE );
  }
#endif

 gtk_adjustment_set_value( HSSubDelayadj,sub_delay );
 gtk_adjustment_set_value( HSSubFPSadj,sub_fps );
 gtk_adjustment_set_value( HSSubPositionadj,sub_pos );
 switch ( osd_level )
  {
   case 0: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBOSDNone ),TRUE ); break;
   case 1: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBOSDIndicator ),TRUE ); break;
   case 2: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBOSDTandP ),TRUE ); break;
   case 3: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBOSDTPTT ),TRUE ); break;
  }
#if 0
 if ( guiInfo.SubtitleFilename ) gtk_entry_set_text( GTK_ENTRY( ESubtitleName ),guiInfo.SubtitleFilename );
#endif

#ifdef CONFIG_ICONV
 if ( sub_cp )
  {
   int i;
   for ( i=0;lEncoding[i].name;i++ )
    if ( !av_strcasecmp( sub_cp,lEncoding[i].name ) ) break;
   if ( lEncoding[i].name ) gtk_entry_set_text( GTK_ENTRY( ESubEncoding ),_(lEncoding[i].comment) );
   else gtk_entry_set_text( GTK_ENTRY( ESubEncoding ),sub_cp );
   gtk_widget_set_sensitive( CBSubUtf8,FALSE );
   gtk_widget_set_sensitive( CBSubUnicode,FALSE );
  }
  else gtk_widget_set_sensitive( CBSubEncoding,(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUtf8)) && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CBSubUnicode))) );
#endif

/* 4th page */
 /* font ... */
 if ( font_name ) gtk_entry_set_text( GTK_ENTRY( prEFontName ),font_name );
#ifdef CONFIG_FREETYPE
 gtk_adjustment_set_value( HSFontBluradj,subtitle_font_radius / 8.0 * 100.0);         // transform 0..8 to 0..100
 gtk_adjustment_set_value( HSFontOutLineadj,subtitle_font_thickness / 8.0 * 100.0);   // transform 0..8 to 0..100
 gtk_adjustment_set_value( HSFontTextScaleadj,text_font_scale_factor );
 gtk_adjustment_set_value( HSFontOSDScaleadj,osd_font_scale_factor );
 {
  int i;
  const char *s = (subtitle_font_encoding ? subtitle_font_encoding : ENC_UNICODE);
  for ( i=0;lEncoding[i].name;i++ )
   if ( !av_strcasecmp( s,lEncoding[i].name ) ) break;
  if ( lEncoding[i].name ) gtk_entry_set_text( GTK_ENTRY( EFontEncoding ),_(lEncoding[i].comment) );
  else gtk_entry_set_text( GTK_ENTRY( EFontEncoding ),s );
 }
 switch ( subtitle_autoscale )
  {
   case 0: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBFontNoAutoScale ),TRUE ); break;
   case 1: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleHeight ),TRUE ); break;
   case 2: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleWidth ),TRUE ); break;
   case 3: gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBFontAutoScaleDiagonal ),TRUE ); break;
  }
#else
 gtk_adjustment_set_value( HSFontFactoradj,font_factor );
#endif

/* 5th page */
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBNonInterlaved ),force_ni );
 if ( index_mode == 1 ) gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBIndex ),1 );
 {
  int     i;
  GList * Items = NULL;
  const char  * name = NULL;

  Items=g_list_append( Items,_(MSGTR_GUI__Default_) );
  for( i=0;mpcodecs_vd_drivers[i];i++ )
   {
    Items=g_list_append( Items,(char *)mpcodecs_vd_drivers[i]->info->name );
    if ( video_fm_list && !gstrcmp( video_fm_list[0],mpcodecs_vd_drivers[i]->info->short_name ) ) name=mpcodecs_vd_drivers[i]->info->name;
   }
  gtk_combo_set_popdown_strings( GTK_COMBO( CBVFM ),Items );
  g_list_free( Items );
  if ( name ) gtk_entry_set_text( GTK_ENTRY( EVFM ),name );
 }

 {
  int     i;
  GList * Items = NULL;
  const char  * name = NULL;

  Items=g_list_append( Items,_(MSGTR_GUI__Default_) );
  for( i=0;mpcodecs_ad_drivers[i];i++ )
   {
    Items=g_list_append( Items,(char *)mpcodecs_ad_drivers[i]->info->name );
    if ( audio_fm_list && !gstrcmp( audio_fm_list[0],mpcodecs_ad_drivers[i]->info->short_name ) ) name=mpcodecs_ad_drivers[i]->info->name;
   }
  gtk_combo_set_popdown_strings( GTK_COMBO( CBAFM ),Items );
  g_list_free( Items );
  if ( name ) gtk_entry_set_text( GTK_ENTRY( EAFM ),name );
 }

/* 6th page */
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBPostprocess ),gtkVfPP );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBLoadFullscreen ),fullscreen );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBSaveWinPos ),gui_save_pos );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBShowVideoWindow ),!gtkShowVideoWindow );
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBStopXScreenSaver ),stop_xscreensaver );
 gtk_adjustment_set_value( HSPPQualityadj,auto_quality );

 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBPlayBar ),gtkEnablePlayBar );
 if ( !guiApp.playbarIsPresent )
  {
   gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBPlayBar ),0 );
   gtk_widget_set_sensitive( CBPlayBar,FALSE );
  }
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBTVDigital ),gui_tv_digital );
#ifndef CONFIG_TV
   gtk_widget_set_sensitive( CBTVDigital,FALSE );
#endif
 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBNoIdle ),!player_idle_mode );
 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CBPlaylists), allow_playlist_parsing);

 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBCache ),0 );
 gtk_adjustment_set_value( SBCacheadj,gtkCacheSize );
 if ( !gtkCacheOn ) gtk_widget_set_sensitive( SBCache,FALSE );
  else gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBCache ),TRUE );

 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBAutoSync ),0 );
 gtk_adjustment_set_value( SBAutoSyncadj,gtkAutoSync );
 if ( !gtkAutoSyncOn ) gtk_widget_set_sensitive( SBAutoSync,FALSE );
  else gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( CBAutoSync ),TRUE );

 if ( dvd_device ) gtk_entry_set_text( GTK_ENTRY( prEDVDDevice ),dvd_device );
  else gtk_entry_set_text( GTK_ENTRY( prEDVDDevice ),DEFAULT_DVD_DEVICE );
 if ( cdrom_device ) gtk_entry_set_text( GTK_ENTRY( prECDRomDevice ),cdrom_device );
  else gtk_entry_set_text( GTK_ENTRY( prECDRomDevice ),DEFAULT_CDROM_DEVICE );

/* disables */
#ifndef CONFIG_ASS
 gtk_widget_set_sensitive( CBUseASS,FALSE );
 gtk_widget_set_sensitive( CBASSUseMargins,FALSE );
 gtk_widget_set_sensitive( SBASSTopMargin,FALSE );
 gtk_widget_set_sensitive( SBASSBottomMargin,FALSE );
#endif

/* signals */
 gtk_signal_connect( GTK_OBJECT( CBExtraStereo ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(0) );
 gtk_signal_connect( GTK_OBJECT( CBNormalize ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(1) );
 gtk_signal_connect( GTK_OBJECT( CBSoftwareMixer ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(1) );
 gtk_signal_connect( GTK_OBJECT( CBAudioEqualizer ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(2) );
 gtk_signal_connect( GTK_OBJECT( CBShowVideoWindow ),"toggled",GTK_SIGNAL_FUNC( prToggled ), GINT_TO_POINTER(3) );
#ifdef CONFIG_FREETYPE
 gtk_signal_connect( GTK_OBJECT( RBFontNoAutoScale ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(4) );
 gtk_signal_connect( GTK_OBJECT( RBFontAutoScaleHeight ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(5) );
 gtk_signal_connect( GTK_OBJECT( RBFontAutoScaleWidth ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(6) );
 gtk_signal_connect( GTK_OBJECT( RBFontAutoScaleDiagonal ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(7) );
#endif
 gtk_signal_connect( GTK_OBJECT( CBCache ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(8));
 gtk_signal_connect( GTK_OBJECT( CBAutoSync ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(9));
#ifdef CONFIG_ASS
 gtk_signal_connect( GTK_OBJECT( CBUseASS ),"toggled",GTK_SIGNAL_FUNC( prToggled ),GINT_TO_POINTER(10));
#endif
 gtk_signal_connect(GTK_OBJECT(CBReplayGain), "toggled", GTK_SIGNAL_FUNC(prToggled), GINT_TO_POINTER(11));

 gtk_signal_connect( GTK_OBJECT( HSExtraStereoMul ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(0) );
 gtk_signal_connect( GTK_OBJECT( HSAudioDelay ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(1) );
 gtk_signal_connect( GTK_OBJECT( HSPanscan ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(2) );
 gtk_signal_connect( GTK_OBJECT( HSSubDelay ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(3) );
 gtk_signal_connect( GTK_OBJECT( HSSubPosition ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(4) );
#ifdef CONFIG_FREETYPE
 gtk_signal_connect( GTK_OBJECT( HSFontBlur ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(6) );
 gtk_signal_connect( GTK_OBJECT( HSFontOutLine ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(7) );
 gtk_signal_connect( GTK_OBJECT( HSFontTextScale ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(8) );
 gtk_signal_connect( GTK_OBJECT( HSFontOSDScale ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(9) );
 gtk_signal_connect( GTK_OBJECT( EFontEncoding ),"changed",GTK_SIGNAL_FUNC( prEntry ),GINT_TO_POINTER(0) );
#else
 gtk_signal_connect( GTK_OBJECT( HSFontFactor ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(5) );
#endif
#ifdef CONFIG_ICONV
 gtk_signal_connect( GTK_OBJECT( ESubEncoding ),"changed",GTK_SIGNAL_FUNC( prEntry ),GINT_TO_POINTER(1) );
#endif
 gtk_signal_connect( GTK_OBJECT( HSPPQuality ),"motion-notify-event",GTK_SIGNAL_FUNC( prHScaler ),GINT_TO_POINTER(10) );

 gtk_signal_connect( GTK_OBJECT( CLADrivers ),"select-row",GTK_SIGNAL_FUNC( prCListRow ),GINT_TO_POINTER(0) );
 gtk_signal_connect( GTK_OBJECT( CLVDrivers ),"select-row",GTK_SIGNAL_FUNC( prCListRow ),GINT_TO_POINTER(1) );

 gtk_widget_show( Preferences );
 gtkSetLayer( Preferences );
}

#ifdef CONFIG_OSS_AUDIO
static GList *appendOSSDevices(GList *l)
{
  /* careful! the current implementation allows only string constants! */
  l = g_list_append(l, (gpointer)"/dev/dsp");
  if (gtkAOOSSDevice && strncmp(gtkAOOSSDevice, "/dev/sound", 10) == 0) {
    l = g_list_append(l, (gpointer)"/dev/sound/dsp0");
    l = g_list_append(l, (gpointer)"/dev/sound/dsp1");
    l = g_list_append(l, (gpointer)"/dev/sound/dsp2");
    l = g_list_append(l, (gpointer)"/dev/sound/dsp3");
  } else {
    l = g_list_append(l, (gpointer)"/dev/dsp0");
    l = g_list_append(l, (gpointer)"/dev/dsp1");
    l = g_list_append(l, (gpointer)"/dev/dsp2");
    l = g_list_append(l, (gpointer)"/dev/dsp3");
  }
#ifdef CONFIG_DXR3
  l = g_list_append(l, (gpointer)"/dev/em8300_ma");
  l = g_list_append(l, (gpointer)"/dev/em8300_ma-0");
  l = g_list_append(l, (gpointer)"/dev/em8300_ma-1");
  l = g_list_append(l, (gpointer)"/dev/em8300_ma-2");
  l = g_list_append(l, (gpointer)"/dev/em8300_ma-3");
#endif
  return l;
}

static GList *appendOSSMixers(GList *l)
{
  /* careful! the current implementation allows only string constants! */
  l = g_list_append(l, (gpointer)"/dev/mixer");
  if (gtkAOOSSMixer && strncmp(gtkAOOSSMixer, "/dev/sound", 10) == 0) {
    l = g_list_append(l, (gpointer)"/dev/sound/mixer0");
    l = g_list_append(l, (gpointer)"/dev/sound/mixer1");
    l = g_list_append(l, (gpointer)"/dev/sound/mixer2");
    l = g_list_append(l, (gpointer)"/dev/sound/mixer3");
  } else {
    l = g_list_append(l, (gpointer)"/dev/mixer0");
    l = g_list_append(l, (gpointer)"/dev/mixer1");
    l = g_list_append(l, (gpointer)"/dev/mixer2");
    l = g_list_append(l, (gpointer)"/dev/mixer3");
  }
  return l;
}

static GList *appendOSSMixerChannels(GList *l)
{
  l = g_list_append(l, (gpointer)"vol");
  l = g_list_append(l, (gpointer)"pcm");
  l = g_list_append(l, (gpointer)"line");
  return l;
}
#endif

#ifdef CONFIG_ALSA
static GList *appendALSADevices(GList *l)
{
  l = g_list_append(l, (gpointer)"default");
  l = g_list_append(l, (gpointer)"hw=0.0");
  l = g_list_append(l, (gpointer)"hw=0.1");
  l = g_list_append(l, (gpointer)"hw=0.2");
  l = g_list_append(l, (gpointer)"surround40");
  l = g_list_append(l, (gpointer)"surround51");
  l = g_list_append(l, (gpointer)"plug=surround40");
  l = g_list_append(l, (gpointer)"plug=surround51");
  return l;
}

static GList *appendALSAMixers(GList *l)
{
  l = g_list_append(l, (gpointer)"default");
  return l;
}

static GList *appendALSAMixerChannels(GList *l)
{
  l = g_list_append(l, (gpointer)"Master");
  l = g_list_append(l, (gpointer)"Line");
  l = g_list_append(l, (gpointer)"PCM");
  l = g_list_append(l, (gpointer)"PCM,1");
  return l;
}
#endif

#ifdef CONFIG_SDL
static GList *appendSDLDevices(GList *l)
{
  l = g_list_append(l, (gpointer)"alsa");
  l = g_list_append(l, (gpointer)"arts");
  l = g_list_append(l, (gpointer)"esd");
  l = g_list_append(l, (gpointer)"jack");
  l = g_list_append(l, (gpointer)"oss");
  l = g_list_append(l, (gpointer)"nas");
  return l;
}
#endif

#ifdef CONFIG_ESD
static GList *appendESDDevices(GList *l)
{
  l = g_list_append(l, (gpointer)"Enter Remote IP");
  l = g_list_append(l, (gpointer)"Use Software Mixer");
  return l;
}
#endif

#if defined(CONFIG_OSS_AUDIO) || defined(CONFIG_ALSA) || defined(CONFIG_SDL) || defined (CONFIG_ESD)
// Gets text string from a gtk entry, interpreting
// _(MSGTR_GUI_DefaultSetting) as null string.
static const char *getGtkEntryText(GtkWidget *from)
{
  const char *tmp = gtk_entry_get_text(GTK_ENTRY(from));
  if (strcmp(tmp, _(MSGTR_GUI_DefaultSetting)) == 0) {
    tmp = NULL;
  }
  return tmp;
}

// Sets text string of a gtk entry, interpreting
// null string as _(MSGTR_GUI_DefaultSetting).
static void setGtkEntryText(GtkWidget *dest, char *to)
{
  if (!to) {
    to = _(MSGTR_GUI_DefaultSetting);
  }
  gtk_entry_set_text(GTK_ENTRY(dest),to);
}
#endif

static GtkWidget *CEAudioDevice;
static GtkWidget *CBAudioDevice;
static GtkWidget *CEAudioMixer;
static GtkWidget *CBAudioMixer;
static GtkWidget *CEAudioMixerChannel;
static GtkWidget *CBAudioMixerChannel;
static GtkWidget *BAudioOk;
static GtkWidget *BAudioCancel;

static void audioButton(GtkButton *button, gpointer user_data) {

  (void) button;

  switch( GPOINTER_TO_INT(user_data) ) {
    case 1:
#ifdef CONFIG_OSS_AUDIO
      if (strncmp(ao_driver[0], "oss", 3) == 0) {
        nfree(gtkAOOSSDevice);
        gtkAOOSSDevice = gstrdup(getGtkEntryText(CEAudioDevice));
        nfree(gtkAOOSSMixer);
        gtkAOOSSMixer = gstrdup(getGtkEntryText(CEAudioMixer));
        nfree(gtkAOOSSMixerChannel);
        gtkAOOSSMixerChannel = gstrdup(getGtkEntryText(CEAudioMixerChannel));
      }
#endif
#ifdef CONFIG_ALSA
      if (strncmp(ao_driver[0], "alsa", 4) == 0) {
        nfree(gtkAOALSADevice);
        gtkAOALSADevice = gstrdup(getGtkEntryText(CEAudioDevice));
        nfree(gtkAOALSAMixer);
        gtkAOALSAMixer = gstrdup(getGtkEntryText(CEAudioMixer));
        nfree(gtkAOALSAMixerChannel);
        gtkAOALSAMixerChannel = gstrdup(getGtkEntryText(CEAudioMixerChannel));
      }
#endif
#ifdef CONFIG_SDL
      if (strncmp(ao_driver[0], "sdl", 3) == 0) {
        nfree(gtkAOSDLDriver);
        gtkAOSDLDriver = gstrdup(getGtkEntryText(CEAudioDevice));
      }
#endif
#ifdef CONFIG_ESD
      if (strncmp(ao_driver[0], "esd", 3) == 0) {
        nfree(gtkAOESDDevice);
        gtkAOESDDevice = gstrdup(getGtkEntryText(CEAudioDevice));
      }
#endif
   case 0:
      gtk_widget_destroy(AudioConfig);
      break;
  }
}

static GtkWidget *CreateAudioConfig( void ) {
  GList *items = NULL;
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *hbuttonbox;
  GtkAccelGroup *accel_group;

  AudioConfig = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(AudioConfig), _(MSGTR_GUI_AudioDriverConfiguration));
  gtk_window_set_position(GTK_WINDOW(AudioConfig), GTK_WIN_POS_CENTER);
  gtk_window_set_policy(GTK_WINDOW(AudioConfig), FALSE, FALSE, TRUE);
  gtk_window_set_wmclass(GTK_WINDOW(AudioConfig), "AudioConfig", MPlayer);

  gtk_widget_realize(AudioConfig);
  gtkAddIcon(AudioConfig);

  vbox = gtkAddVBox(gtkAddDialogFrame(AudioConfig), 0);

  table = gtk_table_new(3, 2, FALSE);
  gtk_widget_show(table);
  gtk_box_pack_start(GTK_BOX(vbox), table, TRUE, TRUE, 0);

  label = gtkAddLabelColon(_(MSGTR_GUI_Device), NULL);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, (GtkAttachOptions)(GTK_FILL), (GtkAttachOptions)(0), 0, 0);

  CBAudioDevice = gtkAddCombo(NULL);
  gtk_table_attach(GTK_TABLE(table), CBAudioDevice, 1, 2, 0, 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(0), 0, 0);
  items = g_list_append(items,(gpointer)_(MSGTR_GUI_DefaultSetting));
#ifdef CONFIG_OSS_AUDIO
  if (strncmp(ao_driver[0], "oss", 3) == 0)
    items = appendOSSDevices(items);
#endif
#ifdef CONFIG_ALSA
  if (strncmp(ao_driver[0], "alsa", 4) == 0)
    items = appendALSADevices(items);
#endif
#ifdef CONFIG_SDL
  if (strncmp(ao_driver[0], "sdl", 3) == 0)
    items = appendSDLDevices(items);
#endif
#ifdef CONFIG_ESD
  if (strncmp(ao_driver[0], "esd", 3) == 0)
    items = appendESDDevices(items);
#endif
  gtk_combo_set_popdown_strings(GTK_COMBO(CBAudioDevice), items);
  g_list_free(items);
  items = NULL;

  CEAudioDevice = GTK_COMBO(CBAudioDevice)->entry;
  gtk_widget_show(CEAudioDevice);

  label = gtkAddLabelColon(_(MSGTR_GUI_Mixer), NULL);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, (GtkAttachOptions)(GTK_FILL), (GtkAttachOptions)(0), 0, 0);

  CBAudioMixer = gtkAddCombo(NULL);
  gtk_table_attach(GTK_TABLE(table), CBAudioMixer, 1, 2, 1, 2, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(0), 0, 0);
  items = g_list_append(items, (gpointer)_(MSGTR_GUI_DefaultSetting));
#ifdef CONFIG_OSS_AUDIO
  if (strncmp(ao_driver[0], "oss", 3) == 0)
    items = appendOSSMixers(items);
#endif
#ifdef CONFIG_ALSA
  if (strncmp(ao_driver[0], "alsa", 4) == 0)
    items = appendALSAMixers(items);
#endif
  gtk_combo_set_popdown_strings(GTK_COMBO(CBAudioMixer), items);
  g_list_free(items);
  items = NULL;

  CEAudioMixer = GTK_COMBO(CBAudioMixer)->entry;
  gtk_widget_show(CEAudioMixer);

  label = gtkAddLabelColon(_(MSGTR_GUI_MixerChannel), NULL);
  gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, (GtkAttachOptions)(GTK_FILL), (GtkAttachOptions)(0), 0, 0);

  CBAudioMixerChannel = gtkAddCombo(NULL);
  gtk_table_attach(GTK_TABLE(table), CBAudioMixerChannel, 1, 2, 2, 3, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)(0), 0, 0);
  items = g_list_append(items, (gpointer)_(MSGTR_GUI_DefaultSetting));
#ifdef CONFIG_OSS_AUDIO
  if (strncmp(ao_driver[0], "oss", 3) == 0)
    items = appendOSSMixerChannels(items);
#endif
#ifdef CONFIG_ALSA
  if (strncmp(ao_driver[0], "alsa", 4) == 0)
    items = appendALSAMixerChannels(items);
#endif
  gtk_combo_set_popdown_strings(GTK_COMBO(CBAudioMixerChannel), items);
  g_list_free(items);
  items = NULL;

  CEAudioMixerChannel = GTK_COMBO(CBAudioMixerChannel)->entry;
  gtk_widget_show(CEAudioMixerChannel);

  gtkAddHSeparator(vbox);

  hbuttonbox = gtkAddHButtonBox(vbox);
  gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonbox), GTK_BUTTONBOX_END);
  gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbuttonbox), 10);
  BAudioOk = gtkAddButton(_(MSGTR_GUI_Ok), hbuttonbox);
  BAudioCancel = gtkAddButton(_(MSGTR_GUI_Cancel), hbuttonbox);

  gtk_signal_connect(GTK_OBJECT(AudioConfig), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &AudioConfig);
  gtk_signal_connect(GTK_OBJECT(BAudioOk), "clicked", GTK_SIGNAL_FUNC(audioButton), GINT_TO_POINTER(1));
  gtk_signal_connect(GTK_OBJECT(BAudioCancel), "clicked", GTK_SIGNAL_FUNC(audioButton), GINT_TO_POINTER(0));

  accel_group = gtk_accel_group_new();
  gtk_widget_add_accelerator(BAudioOk, "clicked", accel_group, GDK_Return, 0, GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator(BAudioCancel, "clicked", accel_group, GDK_Escape, 0, GTK_ACCEL_VISIBLE);
  gtk_window_add_accel_group(GTK_WINDOW(AudioConfig), accel_group);

  return AudioConfig;
}

void ShowAudioConfig( void ) {
  if (AudioConfig) gtkRaise(AudioConfig);
  else AudioConfig = CreateAudioConfig();

#ifdef CONFIG_OSS_AUDIO
  if (strncmp(ao_driver[0], "oss", 3) == 0) {
    setGtkEntryText(CEAudioDevice, gtkAOOSSDevice);
    setGtkEntryText(CEAudioMixer, gtkAOOSSMixer);
    setGtkEntryText(CEAudioMixerChannel, gtkAOOSSMixerChannel);
  }
#endif
#ifdef CONFIG_ALSA
  if (strncmp(ao_driver[0], "alsa", 4) == 0) {
    setGtkEntryText(CEAudioDevice, gtkAOALSADevice);
    setGtkEntryText(CEAudioMixer, gtkAOALSAMixer);
    setGtkEntryText(CEAudioMixerChannel, gtkAOALSAMixerChannel);
  }
#endif
#ifdef CONFIG_SDL
  if (strncmp(ao_driver[0], "sdl", 3) == 0) {
    setGtkEntryText(CEAudioDevice, gtkAOSDLDriver);
  }
#endif
#ifdef CONFIG_ESD
  if (strncmp(ao_driver[0], "esd", 3) == 0) {
    setGtkEntryText(CEAudioDevice, gtkAOESDDevice);
  }
#endif

  gtk_widget_show(AudioConfig);
  gtkSetLayer(AudioConfig);
}

/* dxr3 config box */

static GtkWidget * CBDevice;
static GtkWidget * CEDXR3Device;
static GtkWidget * RBVNone;
static GtkWidget * RBVLavc;
static GtkWidget * dxr3BOk;
static GtkWidget * dxr3BCancel;

GtkWidget * CreateDXR3Config( void );

void ShowDXR3Config( void )
{
 if ( DXR3Config ) gtkRaise( DXR3Config );
  else DXR3Config=CreateDXR3Config();

 gtk_entry_set_text( GTK_ENTRY( CEDXR3Device ),gtkDXR3Device );

 gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBVNone ),TRUE );
 if ( gtkVfLAVC ) gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( RBVLavc ),TRUE );

 gtk_widget_show( DXR3Config );
 gtkSetLayer( DXR3Config );
}

static void dxr3Button( GtkButton * button,gpointer user_data )
{
 (void) button;

 switch ( GPOINTER_TO_INT(user_data) )
 {
  case 0: // Ok
       nfree( gtkDXR3Device ); gtkDXR3Device=strdup( gtk_entry_get_text( GTK_ENTRY( CEDXR3Device ) ) );
       gtkVfLAVC=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( RBVLavc ) );
  case 1: // Cancel
       gtk_widget_destroy( DXR3Config );
       break;
 }
}

GtkWidget * CreateDXR3Config( void )
{
 GtkWidget * vbox1;
 GtkWidget * vbox2;
 GtkWidget * hbox1;
 GList     * CBDevice_items = NULL;
 GtkWidget * vbox3;
 GSList    * VEncoder_group = NULL;
 GtkWidget * hbuttonbox1;
 GtkAccelGroup * accel_group;

 accel_group=gtk_accel_group_new();

 DXR3Config=gtk_window_new( GTK_WINDOW_TOPLEVEL );
// gtk_widget_set_usize( DXR3Config,300,156 );
 gtk_window_set_title( GTK_WINDOW( DXR3Config ),"DXR3/H+" );
 gtk_window_set_position( GTK_WINDOW( DXR3Config ),GTK_WIN_POS_CENTER );
 gtk_window_set_policy( GTK_WINDOW( DXR3Config ),FALSE,FALSE,TRUE );
 gtk_window_set_wmclass( GTK_WINDOW( DXR3Config ),"DXR3Config",MPlayer );

 gtk_widget_realize( DXR3Config );
 gtkAddIcon( DXR3Config );

 vbox1=gtkAddVBox( gtkAddDialogFrame( DXR3Config ),0 );
 vbox2=gtkAddVBox( vbox1,0 );
 hbox1=gtkAddHBox( vbox2,1 );
 gtkAddLabelColon( _(MSGTR_GUI_Device),hbox1 );

 CBDevice=gtkAddCombo( hbox1 );

 CBDevice_items=g_list_append( CBDevice_items,( gpointer ) "/dev/em8300" );
 CBDevice_items=g_list_append( CBDevice_items,( gpointer ) "/dev/em8300-0" );
 CBDevice_items=g_list_append( CBDevice_items,( gpointer ) "/dev/em8300-1" );
 CBDevice_items=g_list_append( CBDevice_items,( gpointer ) "/dev/em8300-2" );
 CBDevice_items=g_list_append( CBDevice_items,( gpointer ) "/dev/em8300-3" );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBDevice ),CBDevice_items );
 g_list_free( CBDevice_items );

 CEDXR3Device=GTK_COMBO( CBDevice )->entry;
 gtk_widget_show( CEDXR3Device );
 gtk_entry_set_text( GTK_ENTRY( CEDXR3Device ),"/dev/em8300" );

 gtkAddHSeparator( vbox2 );
 vbox3=gtkAddVBox( vbox2,0 );
 gtkAddLabelColon( _(MSGTR_GUI_VideoEncoder),vbox3 );
 RBVNone=gtkAddRadioButton( _(MSGTR_GUI_None),&VEncoder_group,vbox3 );
 RBVLavc=gtkAddRadioButton( _(MSGTR_GUI_Lavc),&VEncoder_group,vbox3 );

 gtkAddHSeparator( vbox1 );

 hbuttonbox1=gtkAddHButtonBox( vbox1 );
   gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
   gtk_button_box_set_spacing( GTK_BUTTON_BOX( hbuttonbox1 ),10 );
 dxr3BOk=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );
 dxr3BCancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox1 );

 gtk_widget_add_accelerator( dxr3BOk,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );
 gtk_widget_add_accelerator( dxr3BCancel,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );

 gtk_signal_connect( GTK_OBJECT( DXR3Config ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&DXR3Config );

 gtk_signal_connect( GTK_OBJECT( dxr3BOk ),"clicked",GTK_SIGNAL_FUNC( dxr3Button ),GINT_TO_POINTER(0) );
 gtk_signal_connect( GTK_OBJECT( dxr3BCancel ),"clicked",GTK_SIGNAL_FUNC( dxr3Button ),GINT_TO_POINTER(1) );

 gtk_window_add_accel_group( GTK_WINDOW( DXR3Config ),accel_group );

 return DXR3Config;
}
