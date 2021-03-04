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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "gui/app/app.h"
#include "gui/app/cfg.h"
#include "gui/app/gui.h"
#include "gui/interface.h"
#include "help_mp.h"
#include "libaf/equalizer.h"
#include "libavutil/common.h"
#include "libvo/video_out.h"
#include "stream/stream.h"
#include "libmpdemux/demuxer.h"
#include "libmpdemux/stheader.h"
#include "libmpcodecs/dec_video.h"
#include "dialog.h"
#include "gui/util/mem.h"
#include "gui/util/string.h"

#include "equalizer.h"
#include "tools.h"

#define eqRange 15

GtkWidget * Equalizer = NULL;
static GtkWidget * EquConfig;

static GtkWidget * Notebook;
static GtkWidget * ChannelsList;
static GtkWidget * VContrast, * VBrightness, * VHue, * VSaturation;
static GtkAdjustment * VContrastadj, * VBrightnessadj, * VHueadj, * VSaturationadj;
static GtkWidget * Ok, * Clear, * Config;
static GtkWidget * A3125,  * A125, * A6250, * A250, * A500, * A1000, * A2000, * A4000, * A8000, * A16000;
static GtkAdjustment * A3125adj, * A125adj, * A6250adj, * A250adj, * A500adj, * A1000adj, * A2000adj, * A4000adj, * A8000adj, * A16000adj;

static int Channel = -1;

// ---

char * gtkEquChannel1 = NULL;
char * gtkEquChannel2 = NULL;
char * gtkEquChannel3 = NULL;
char * gtkEquChannel4 = NULL;
char * gtkEquChannel5 = NULL;
char * gtkEquChannel6 = NULL;

// ---

void ShowEquConfig( void );

static void eqSetBands( int channel )
{
 if ( channel < 0 ) channel=0;
 gtk_adjustment_set_value( A3125adj,-gtkEquChannels[channel][0] );
 gtk_adjustment_set_value( A6250adj,-gtkEquChannels[channel][1] );
 gtk_adjustment_set_value( A125adj,-gtkEquChannels[channel][2] );
 gtk_adjustment_set_value( A250adj,-gtkEquChannels[channel][3] );
 gtk_adjustment_set_value( A500adj,-gtkEquChannels[channel][4] );
 gtk_adjustment_set_value( A1000adj,-gtkEquChannels[channel][5] );
 gtk_adjustment_set_value( A2000adj,-gtkEquChannels[channel][6] );
 gtk_adjustment_set_value( A4000adj,-gtkEquChannels[channel][7] );
 gtk_adjustment_set_value( A8000adj,-gtkEquChannels[channel][8] );
 gtk_adjustment_set_value( A16000adj,-gtkEquChannels[channel][9] );

 if ( guiInfo.sh_video )
  {
   get_video_colors( guiInfo.sh_video,"brightness",&vo_gamma_brightness );
   get_video_colors( guiInfo.sh_video,"contrast",&vo_gamma_contrast );
   get_video_colors( guiInfo.sh_video,"hue",&vo_gamma_hue );
   get_video_colors( guiInfo.sh_video,"saturation",&vo_gamma_saturation );
  }

 gtk_adjustment_set_value( VContrastadj,vo_gamma_contrast );
 gtk_adjustment_set_value( VBrightnessadj,vo_gamma_brightness );
 gtk_adjustment_set_value( VHueadj,vo_gamma_hue );
 gtk_adjustment_set_value( VSaturationadj,vo_gamma_saturation );
}

static void eqSetChannelNames( void )
{
 gchar * str[2];
 gtk_clist_clear( GTK_CLIST( ChannelsList ) );
 str[1]="";
 str[0]=_(MSGTR_GUI_ChannelAll);
 gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
 if ( guiInfo.AudioChannels > 1 )
  {
   str[0]=gtkEquChannel1; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
   str[0]=gtkEquChannel2; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
  }
 if ( guiInfo.AudioChannels > 2 )
  {
   str[0]=gtkEquChannel3; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
   str[0]=gtkEquChannel4; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
  }
 if ( guiInfo.AudioChannels > 4 )
  {
   str[0]=gtkEquChannel5; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
   str[0]=gtkEquChannel6; gtk_clist_append( GTK_CLIST( ChannelsList ) ,str);
  }
 gtk_clist_select_row( GTK_CLIST( ChannelsList ),0,0 );
}

static gboolean eqHScaleMotion( GtkWidget * widget,GdkEventMotion  * event,gpointer user_data )
{
 equalizer_t eq;

 (void) widget;
 (void) event;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case 0: eq.gain=A3125adj->value; break;
   case 1: eq.gain=A6250adj->value; break;
   case 2: eq.gain=A125adj->value; break;
   case 3: eq.gain=A250adj->value; break;
   case 4: eq.gain=A500adj->value; break;
   case 5: eq.gain=A1000adj->value; break;
   case 6: eq.gain=A2000adj->value; break;
   case 7: eq.gain=A4000adj->value; break;
   case 8: eq.gain=A8000adj->value; break;
   case 9: eq.gain=A16000adj->value; break;
   default: return FALSE;
  }
 eq.gain=-eq.gain;
 eq.band=GPOINTER_TO_INT(user_data);
 if ( Channel == -1 )
  {
   int i;
   for ( i=0;i<6;i++ )
    { eq.channel=i; mplayer( MPLAYER_SET_EQUALIZER,0,&eq ); }
  } else { eq.channel=Channel; mplayer( MPLAYER_SET_EQUALIZER,0,&eq ); }

 return FALSE;
}

static gboolean eqVScaleMotion( GtkWidget * widget,GdkEventMotion  * event,gpointer user_data )
{
 (void) widget;
 (void) event;

 switch( GPOINTER_TO_INT(user_data) )
  {
   case 1: mplayer( MPLAYER_SET_CONTRAST,VContrastadj->value,0 );      break;
   case 2: mplayer( MPLAYER_SET_BRIGHTNESS,VBrightnessadj->value,0 );  break;
   case 3: mplayer( MPLAYER_SET_HUE,VHueadj->value,0 );                break;
   case 4: mplayer( MPLAYER_SET_SATURATION,VSaturationadj->value,0 );  break;
  }

 return FALSE;
}

static void eqButtonReleased( GtkButton * button,gpointer user_data )
{
 (void) button;

 switch( GPOINTER_TO_INT(user_data) )
  {
   case 0:
        gtk_widget_destroy( Equalizer );
        if ( EquConfig ) gtk_widget_destroy( EquConfig );
        break;
   case 1:
        if ( gtk_notebook_get_current_page( GTK_NOTEBOOK( Notebook ) ) == 0 )
         {
          if ( !guiInfo.Playing || !gtkEnableAudioEqualizer ) break;
          mplayer( MPLAYER_SET_EQUALIZER,0,NULL );
          eqSetBands( Channel );
         }
         else
          {
           if ( !guiInfo.Playing ) break;
           mplayer( MPLAYER_SET_CONTRAST,0,0 );
           mplayer( MPLAYER_SET_BRIGHTNESS,0,0 );
           mplayer( MPLAYER_SET_HUE,0,0 );
           mplayer( MPLAYER_SET_SATURATION,0,0 );
           eqSetBands( Channel );
          }
        break;
   case 2:
        ShowEquConfig();
        break;
  }
}

static gboolean eqFocus( GtkWidget * widget,GdkEvent * event,gpointer user_data )
{
 (void) widget;
 (void) event;
 (void) user_data;

 eqSetBands( Channel );
 return FALSE;
}

static void eqSelectChannelsListRow( GtkCList * clist,gint row,gint column,GdkEvent * event,gpointer user_data )
{
 (void) clist;
 (void) column;
 (void) event;
 (void) user_data;

 Channel=row - 1;
 eqSetBands( Channel );
 if ( Channel == -1 )
  {
   unsigned int i,j; equalizer_t eq;
   for ( i=1;i<FF_ARRAY_ELEMS(gtkEquChannels);i++ )
    for ( j=0;j<FF_ARRAY_ELEMS(*gtkEquChannels);j++ )
     { eq.band=j; eq.channel=i; eq.gain=gtkEquChannels[0][j]; mplayer( MPLAYER_SET_EQUALIZER,0,&eq ); }
  }
}

static void eqNotebook( GtkNotebook * notebook, gpointer page,
                        guint page_num, gpointer user_data )
{
 (void) notebook;
 (void) page;
 (void) user_data;

 if ( page_num ) gtk_widget_hide( Config );
   else gtk_widget_show( Config );
}

static GtkWidget * CreateEqualizer( void )
{
  GtkWidget * vbox1;
  GtkWidget * hbox1;
  GtkWidget * scrolledwindow1;
  GtkWidget * table1;
  GtkWidget * hbuttonbox1;
  GtkAccelGroup * accel_group;

  accel_group=gtk_accel_group_new();

  Equalizer=gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_widget_set_usize( Equalizer,-1,256 );
  gtk_window_set_title( GTK_WINDOW( Equalizer ),_(MSGTR_GUI_Equalizer) );
  gtk_window_set_position( GTK_WINDOW( Equalizer ),GTK_WIN_POS_CENTER );
  gtk_window_set_policy( GTK_WINDOW( Equalizer ),FALSE,FALSE,TRUE );
  gtk_window_set_wmclass( GTK_WINDOW( Equalizer ),"Equalizer",MPlayer );

  gtk_widget_realize( Equalizer );
  gtkAddIcon( Equalizer );

  vbox1=gtkAddVBox( gtkAddDialogFrame( Equalizer ),0 );

  Notebook=gtk_notebook_new();
  gtk_widget_show( Notebook );
  gtk_box_pack_start( GTK_BOX( vbox1 ),Notebook,TRUE,TRUE,0 );
  gtk_container_set_border_width( GTK_CONTAINER( Notebook ),1 );

  hbox1=gtkAddHBox( Notebook,0 );

  scrolledwindow1=gtk_scrolled_window_new( NULL,NULL );
  gtk_widget_show( scrolledwindow1 );
  gtk_box_pack_start( GTK_BOX( hbox1 ),scrolledwindow1,FALSE,FALSE,0 );
  gtk_widget_set_usize( scrolledwindow1,106,-2 );
  gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolledwindow1 ),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC );

  ChannelsList=gtk_clist_new( 1 );
  gtk_widget_show( ChannelsList );
  gtk_container_add( GTK_CONTAINER( scrolledwindow1 ),ChannelsList );
  gtk_clist_set_column_width( GTK_CLIST( ChannelsList ),0,80 );
  gtk_clist_column_titles_hide( GTK_CLIST( ChannelsList ) );

  table1=gtk_table_new( 2,10,FALSE );
  gtk_widget_show( table1 );
  gtk_box_pack_start( GTK_BOX( hbox1 ),table1,FALSE,FALSE,0 );
  gtk_table_set_row_spacings( GTK_TABLE( table1 ),4 );
  gtk_table_set_col_spacings( GTK_TABLE( table1 ),9 );

  A3125adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A3125=gtkAddVScale( A3125adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A3125,0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A6250adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A6250=gtkAddVScale( A6250adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A6250,1,2,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A125adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A125=gtkAddVScale( A125adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A125,2,3,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A250adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A250=gtkAddVScale( A250adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A250,3,4,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A500adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A500=gtkAddVScale( A500adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A500,4,5,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A1000adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A1000=gtkAddVScale( A1000adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A1000,5,6,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A2000adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A2000=gtkAddVScale( A2000adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A2000,6,7,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A4000adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A4000=gtkAddVScale( A4000adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A4000,7,8,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A8000adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A8000=gtkAddVScale( A8000adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A8000,8,9,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  A16000adj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-eqRange,eqRange,0.5,0,0 ) );
  A16000=gtkAddVScale( A16000adj,NULL,-1 );
    gtk_table_attach( GTK_TABLE( table1 ),A16000,9,10,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency0),NULL ),
    0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency1),NULL ),
    1,2,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency2),NULL ),
    2,3,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency3),NULL ),
    3,4,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency4),NULL ),
    4,5,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency5),NULL ),
    5,6,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency6),NULL ),
    6,7,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency7),NULL ),
    7,8,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency8),NULL ),
    8,9,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabel( _(MSGTR_GUI_Frequency9),NULL ),
    9,10,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_notebook_set_tab_label( GTK_NOTEBOOK( Notebook ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( Notebook ),0 ),
    gtkAddLabel( _(MSGTR_GUI_Audio),NULL ) );

  table1=gtk_table_new( 4,2,FALSE );
  gtk_widget_show( table1 );
  gtk_container_add( GTK_CONTAINER( Notebook ),table1 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Contrast),NULL ),
    0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Brightness),NULL ),
    0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Hue),NULL ),
    0,1,2,3,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Saturation),NULL ),
    0,1,3,4,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  VContrastadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-100,100,1,0,0 ) );
  VContrast=gtkAddHScale( VContrastadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),VContrast,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_usize( VContrast,-1,45 );

  VBrightnessadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-100,100,1,0,0 ) );
  VBrightness=gtkAddHScale( VBrightnessadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),VBrightness,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_usize( VBrightness,-1,45 );

  VHueadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-100,100,1,0,0 ) );
  VHue=gtkAddHScale( VHueadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),VHue,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_usize( VHue,-1,45 );

  VSaturationadj=GTK_ADJUSTMENT( gtk_adjustment_new( 0,-100,100,1,0,0 ) );
  VSaturation=gtkAddHScale( VSaturationadj,NULL,1 );
    gtk_table_attach( GTK_TABLE( table1 ),VSaturation,1,2,3,4,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_usize( VSaturation,-1,45 );

  gtk_notebook_set_tab_label( GTK_NOTEBOOK( Notebook ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( Notebook ),1 ),
    gtkAddLabel( _(MSGTR_GUI_Video),NULL ) );

  gtkAddHSeparator( vbox1 );

  hbuttonbox1=gtkAddHButtonBox( vbox1 );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
    gtk_button_box_set_spacing( GTK_BUTTON_BOX( hbuttonbox1 ),10 );

  Config=gtkAddButton( _(MSGTR_GUI_Configure),hbuttonbox1 );
  Clear=gtkAddButton( _(MSGTR_GUI_Clear),hbuttonbox1 );
  Ok=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );

  gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );
  gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );

  gtk_signal_connect( GTK_OBJECT( Equalizer ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&Equalizer );
  gtk_signal_connect( GTK_OBJECT( Equalizer ),"focus-in-event",GTK_SIGNAL_FUNC( eqFocus ),GINT_TO_POINTER(2) );

  gtk_signal_connect( GTK_OBJECT( ChannelsList ),"select-row",GTK_SIGNAL_FUNC( eqSelectChannelsListRow ),NULL );

  gtk_signal_connect( GTK_OBJECT( A3125 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(0) );
  gtk_signal_connect( GTK_OBJECT( A6250 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(1) );
  gtk_signal_connect( GTK_OBJECT( A125 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(2) );
  gtk_signal_connect( GTK_OBJECT( A250 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(3) );
  gtk_signal_connect( GTK_OBJECT( A500 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(4) );
  gtk_signal_connect( GTK_OBJECT( A1000 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(5) );
  gtk_signal_connect( GTK_OBJECT( A2000 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(6) );
  gtk_signal_connect( GTK_OBJECT( A4000 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(7) );
  gtk_signal_connect( GTK_OBJECT( A8000 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(8) );
  gtk_signal_connect( GTK_OBJECT( A16000 ),"motion-notify-event",GTK_SIGNAL_FUNC( eqHScaleMotion ),GINT_TO_POINTER(9) );

  gtk_signal_connect( GTK_OBJECT( VContrast ),"motion-notify-event",GTK_SIGNAL_FUNC( eqVScaleMotion ),GINT_TO_POINTER(1) );
  gtk_signal_connect( GTK_OBJECT( VBrightness ),"motion-notify-event",GTK_SIGNAL_FUNC( eqVScaleMotion ),GINT_TO_POINTER(2) );
  gtk_signal_connect( GTK_OBJECT( VHue ),"motion-notify-event",GTK_SIGNAL_FUNC( eqVScaleMotion ),GINT_TO_POINTER(3) );
  gtk_signal_connect( GTK_OBJECT( VSaturation ),"motion-notify-event",GTK_SIGNAL_FUNC( eqVScaleMotion ),GINT_TO_POINTER(4) );

  gtk_signal_connect( GTK_OBJECT( Ok ),"clicked",GTK_SIGNAL_FUNC( eqButtonReleased ),GINT_TO_POINTER(0) );
  gtk_signal_connect( GTK_OBJECT( Clear ),"clicked",GTK_SIGNAL_FUNC( eqButtonReleased ),GINT_TO_POINTER(1) );
  gtk_signal_connect( GTK_OBJECT( Config ),"clicked",GTK_SIGNAL_FUNC( eqButtonReleased ),GINT_TO_POINTER(2) );

  gtk_signal_connect( GTK_OBJECT( Notebook ),"switch-page",GTK_SIGNAL_FUNC( eqNotebook ),NULL );

  gtk_window_add_accel_group( GTK_WINDOW( Equalizer ),accel_group );

  return Equalizer;
}

void ShowEqualizer( void )
{
 if ( Equalizer ) gtkRaise( Equalizer );
    else Equalizer=CreateEqualizer();

 if ( !gtkEquChannel1 ) gtkEquChannel1=strdup( _(MSGTR_GUI_FrontRight) );
 if ( !gtkEquChannel2 ) gtkEquChannel2=strdup( _(MSGTR_GUI_FrontLeft) );
 if ( !gtkEquChannel3 ) gtkEquChannel3=strdup( _(MSGTR_GUI_RearRight) );
 if ( !gtkEquChannel4 ) gtkEquChannel4=strdup( _(MSGTR_GUI_RearLeft) );
 if ( !gtkEquChannel5 ) gtkEquChannel5=strdup( _(MSGTR_GUI_Center) );
 if ( !gtkEquChannel6 ) gtkEquChannel6=strdup( _(MSGTR_GUI_Bass) );

 eqSetChannelNames();

 if ( !guiInfo.Playing || !guiInfo.sh_video )
  {
   gtk_widget_set_sensitive( VContrast,FALSE );
   gtk_widget_set_sensitive( VBrightness,FALSE );
   gtk_widget_set_sensitive( VHue,FALSE );
   gtk_widget_set_sensitive( VSaturation,FALSE );
  }
 Channel=-1;
 eqSetBands( 0 );
 if ( !guiInfo.Playing || !gtkEnableAudioEqualizer )
  {
   gtk_widget_set_sensitive( ChannelsList,FALSE );
   gtk_widget_set_sensitive( A3125,FALSE );
   gtk_widget_set_sensitive( A125,FALSE );
   gtk_widget_set_sensitive( A6250,FALSE );
   gtk_widget_set_sensitive( A250,FALSE );
   gtk_widget_set_sensitive( A500,FALSE );
   gtk_widget_set_sensitive( A1000,FALSE );
   gtk_widget_set_sensitive( A2000,FALSE );
   gtk_widget_set_sensitive( A4000,FALSE );
   gtk_widget_set_sensitive( A8000,FALSE );
   gtk_widget_set_sensitive( A16000,FALSE );
  }

 if ( gtk_notebook_get_current_page( GTK_NOTEBOOK( Notebook ) ) == 0 ) gtk_widget_show( Config );
 gtk_widget_show( Equalizer );
}

/* equalizer config dialog box */

static GtkWidget * CBChannel1;
static GtkWidget * CEChannel1;
static GtkWidget * CBChannel2;
static GtkWidget * CEChannel2;
static GtkWidget * CBChannel3;
static GtkWidget * CEChannel3;
static GtkWidget * CBChannel4;
static GtkWidget * CEChannel4;
static GtkWidget * CBChannel5;
static GtkWidget * CEChannel5;
static GtkWidget * CBChannel6;
static GtkWidget * CEChannel6;
static GtkWidget * ecOk;
static GtkWidget * ecCancel;

GtkWidget * CreateEquConfig( void );

void ShowEquConfig( void )
{
 GList * Items = NULL;

 if ( EquConfig ) gtkRaise( EquConfig );
    else EquConfig=CreateEquConfig();

 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_FrontRight)  );
 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_FrontLeft) );
 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_RearRight) );
 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_RearLeft) );
 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_Center) );
 Items=g_list_append( Items,(gpointer)_(MSGTR_GUI_Bass) );

 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel1 ),Items );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel2 ),Items );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel3 ),Items );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel4 ),Items );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel5 ),Items );
 gtk_combo_set_popdown_strings( GTK_COMBO( CBChannel6 ),Items );

 g_list_free( Items );

 gtk_entry_set_text( GTK_ENTRY( CEChannel1 ),gtkEquChannel1 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel1 ),FALSE );
 gtk_entry_set_text( GTK_ENTRY( CEChannel2 ),gtkEquChannel2 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel2 ),FALSE );
 gtk_entry_set_text( GTK_ENTRY( CEChannel3 ),gtkEquChannel3 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel3 ),FALSE );
 gtk_entry_set_text( GTK_ENTRY( CEChannel4 ),gtkEquChannel4 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel4 ),FALSE );
 gtk_entry_set_text( GTK_ENTRY( CEChannel5 ),gtkEquChannel5 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel5 ),FALSE );
 gtk_entry_set_text( GTK_ENTRY( CEChannel6 ),gtkEquChannel6 ); gtk_entry_set_editable( GTK_ENTRY( CEChannel6 ),FALSE );

 gtk_widget_show( EquConfig );
 gtkSetLayer( EquConfig );
}

static void ecButtonReleased( GtkButton * button,gpointer user_data )
{
 (void) button;

 if ( GPOINTER_TO_INT(user_data) )
 { // if you pressed Ok
  nfree( gtkEquChannel1 ); gtkEquChannel1=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel1 ) ) );
  nfree( gtkEquChannel2 ); gtkEquChannel2=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel2 ) ) );
  nfree( gtkEquChannel3 ); gtkEquChannel3=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel3 ) ) );
  nfree( gtkEquChannel4 ); gtkEquChannel4=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel4 ) ) );
  nfree( gtkEquChannel5 ); gtkEquChannel5=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel5 ) ) );
  nfree( gtkEquChannel6 ); gtkEquChannel6=gstrdup( gtk_entry_get_text( GTK_ENTRY( CEChannel6 ) ) );
  eqSetChannelNames();
 }
 gtk_widget_destroy( EquConfig );
}

GtkWidget * CreateEquConfig( void )
{
  GtkWidget * vbox1;
  GtkWidget * table1;
  GtkWidget * hbuttonbox1;
  GtkAccelGroup * accel_group;

  accel_group=gtk_accel_group_new();

  EquConfig=gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_widget_set_usize( EquConfig,350,260 );
  gtk_window_set_title( GTK_WINDOW( EquConfig ),_(MSGTR_GUI_EqualizerConfiguration) );
  gtk_window_set_position( GTK_WINDOW( EquConfig ),GTK_WIN_POS_CENTER );
//  gtk_window_set_modal( GTK_WINDOW( EquConfig ),TRUE );
  gtk_window_set_policy( GTK_WINDOW( EquConfig ),FALSE,FALSE,TRUE );
  gtk_window_set_wmclass( GTK_WINDOW( EquConfig ),"EqualizerConfig",MPlayer );

  gtk_widget_realize( EquConfig );
  gtkAddIcon( EquConfig );

  vbox1=gtkAddVBox( gtkAddDialogFrame( EquConfig ),0 );

  table1=gtk_table_new( 6,2,FALSE );
  gtk_widget_show( table1 );
  gtk_box_pack_start( GTK_BOX( vbox1 ),table1,TRUE,TRUE,0 );
  gtk_table_set_row_spacings( GTK_TABLE( table1 ),4 );
  gtk_table_set_col_spacings( GTK_TABLE( table1 ),4 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel1),NULL ),
    0,1,0,1,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel2),NULL ),
    0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel3),NULL ),
    0,1,2,3,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel4),NULL ),
    0,1,3,4,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel5),NULL ),
    0,1,4,5,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    gtkAddLabelColon( _(MSGTR_GUI_Channel6),NULL ),
    0,1,5,6,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CBChannel1=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel1,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel1=GTK_COMBO( CBChannel1 )->entry;
  gtk_widget_show( CEChannel1 );

  CBChannel2=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel2,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel2=GTK_COMBO( CBChannel2 )->entry;
  gtk_widget_show( CEChannel2 );

  CBChannel3=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel3,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel3=GTK_COMBO( CBChannel3 )->entry;
  gtk_widget_show( CEChannel3 );

  CBChannel4=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel4,1,2,3,4,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel4=GTK_COMBO( CBChannel4 )->entry;
  gtk_widget_show( CEChannel4 );

  CBChannel5=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel5,1,2,4,5,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel5=GTK_COMBO( CBChannel5 )->entry;
  gtk_widget_show( CEChannel5 );

  CBChannel6=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel6,1,2,5,6,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  CEChannel6=GTK_COMBO( CBChannel6 )->entry;
  gtk_widget_show( CEChannel6 );

  gtkAddHSeparator( vbox1 );

  hbuttonbox1=gtkAddHButtonBox( vbox1 );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
    gtk_button_box_set_spacing( GTK_BUTTON_BOX( hbuttonbox1 ),10 );

  ecOk=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );
  ecCancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox1 );

  gtk_widget_add_accelerator( ecOk,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );
  gtk_widget_add_accelerator( ecCancel,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );

  gtk_signal_connect( GTK_OBJECT( EquConfig ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&EquConfig );

  gtk_signal_connect( GTK_OBJECT( ecOk ),"clicked",GTK_SIGNAL_FUNC( ecButtonReleased ),GINT_TO_POINTER(1) );
  gtk_signal_connect( GTK_OBJECT( ecCancel ),"clicked",GTK_SIGNAL_FUNC( ecButtonReleased ),GINT_TO_POINTER(0) );

  gtk_window_add_accel_group( GTK_WINDOW( EquConfig ),accel_group );

  return EquConfig;
}
