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

#include "gui/gtk-compat.h"
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

#define eqRange 12

GtkWidget * Equalizer = NULL;
static GtkWidget * EquConfig;

static GtkWidget * Notebook;
static GtkWidget * ChannelsList;
static GtkWidget * VContrast, * VBrightness, * VHue, * VSaturation;
static GtkAdjustment * VContrastadj, * VBrightnessadj, * VHueadj, * VSaturationadj;
static GtkWidget * Ok, * Clear, * Config;
static GtkWidget *Frequency0, *Frequency1, *Frequency2, *Frequency3, *Frequency4, *Frequency5, *Frequency6, *Frequency7, *Frequency8, *Frequency9;
static GtkWidget * A3125,  * A125, * A6250, * A250, * A500, * A1000, * A2000, * A4000, * A8000, * A16000;
static GtkAdjustment * A3125adj, * A125adj, * A6250adj, * A250adj, * A500adj, * A1000adj, * A2000adj, * A4000adj, * A8000adj, * A16000adj;

static int Channel = -1;   // the selected channel, or -1 (all channels)

static float old_gtkEquChannels[6][10];

// ---

char * gtkEquChannel1 = NULL;
char * gtkEquChannel2 = NULL;
char * gtkEquChannel3 = NULL;
char * gtkEquChannel4 = NULL;
char * gtkEquChannel5 = NULL;
char * gtkEquChannel6 = NULL;

// ---

void ShowEquConfig( void );

static void eqHScaleChanged (GtkRange *range, gpointer user_data)
{
 equalizer_t eq;

 (void) range;

 switch ( GPOINTER_TO_INT(user_data) )
  {
   case 0: eq.gain=gtk_adjustment_get_value(A3125adj); break;
   case 1: eq.gain=gtk_adjustment_get_value(A6250adj); break;
   case 2: eq.gain=gtk_adjustment_get_value(A125adj); break;
   case 3: eq.gain=gtk_adjustment_get_value(A250adj); break;
   case 4: eq.gain=gtk_adjustment_get_value(A500adj); break;
   case 5: eq.gain=gtk_adjustment_get_value(A1000adj); break;
   case 6: eq.gain=gtk_adjustment_get_value(A2000adj); break;
   case 7: eq.gain=gtk_adjustment_get_value(A4000adj); break;
   case 8: eq.gain=gtk_adjustment_get_value(A8000adj); break;
   case 9: eq.gain=gtk_adjustment_get_value(A16000adj); break;
   default: return;
  }
 eq.gain=-eq.gain;
 eq.band=GPOINTER_TO_INT(user_data);
 if ( Channel == -1 )
  {
   int i;
   for ( i=0;i<6;i++ )
    { eq.channel=i; mplayer( MPLAYER_SET_EQUALIZER,0,&eq ); }
  } else { eq.channel=Channel; mplayer( MPLAYER_SET_EQUALIZER,0,&eq ); }
}

static void eqSetBands( int channel )
{
 g_signal_handlers_block_by_func(G_OBJECT(A3125), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(0));
 g_signal_handlers_block_by_func(G_OBJECT(A6250), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(1));
 g_signal_handlers_block_by_func(G_OBJECT(A125), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(2));
 g_signal_handlers_block_by_func(G_OBJECT(A250), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(3));
 g_signal_handlers_block_by_func(G_OBJECT(A500), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(4));
 g_signal_handlers_block_by_func(G_OBJECT(A1000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(5));
 g_signal_handlers_block_by_func(G_OBJECT(A2000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(6));
 g_signal_handlers_block_by_func(G_OBJECT(A4000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(7));
 g_signal_handlers_block_by_func(G_OBJECT(A8000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(8));
 g_signal_handlers_block_by_func(G_OBJECT(A16000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(9));

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

 g_signal_handlers_unblock_by_func(G_OBJECT(A16000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(9));
 g_signal_handlers_unblock_by_func(G_OBJECT(A8000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(8));
 g_signal_handlers_unblock_by_func(G_OBJECT(A4000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(7));
 g_signal_handlers_unblock_by_func(G_OBJECT(A2000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(6));
 g_signal_handlers_unblock_by_func(G_OBJECT(A1000), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(5));
 g_signal_handlers_unblock_by_func(G_OBJECT(A500), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(4));
 g_signal_handlers_unblock_by_func(G_OBJECT(A250), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(3));
 g_signal_handlers_unblock_by_func(G_OBJECT(A125), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(2));
 g_signal_handlers_unblock_by_func(G_OBJECT(A6250), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(1));
 g_signal_handlers_unblock_by_func(G_OBJECT(A3125), G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(0));
}

static void eqSetChannelNames( void )
{
 gchar *str[1];
 gtk_clist_clear( GTK_CLIST( ChannelsList ) );
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
}

static void eqVScaleChanged (GtkRange *range, gpointer user_data)
{
 (void) range;

 switch( GPOINTER_TO_INT(user_data) )
  {
   case 1: mplayer( MPLAYER_SET_CONTRAST,gtk_adjustment_get_value(VContrastadj),0 );      break;
   case 2: mplayer( MPLAYER_SET_BRIGHTNESS,gtk_adjustment_get_value(VBrightnessadj),0 );  break;
   case 3: mplayer( MPLAYER_SET_HUE,gtk_adjustment_get_value(VHueadj),0 );                break;
   case 4: mplayer( MPLAYER_SET_SATURATION,gtk_adjustment_get_value(VSaturationadj),0 );  break;
  }
}

static void eqButtonReleased( GtkButton * button,gpointer user_data )
{
 (void) button;

 switch( GPOINTER_TO_INT(user_data) )
  {
   case 0:
        if (guiInfo.sh_video)
         {
          get_video_colors(guiInfo.sh_video, "brightness", &vo_gamma_brightness);
          get_video_colors(guiInfo.sh_video, "contrast", &vo_gamma_contrast);
          get_video_colors(guiInfo.sh_video, "hue", &vo_gamma_hue);
          get_video_colors(guiInfo.sh_video, "saturation", &vo_gamma_saturation);
         }
         // fall through

   case 9:
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
           gtk_adjustment_set_value(VContrastadj, 0);

           mplayer( MPLAYER_SET_BRIGHTNESS,0,0 );
           gtk_adjustment_set_value(VBrightnessadj, 0);

           mplayer( MPLAYER_SET_HUE,0,0 );
           gtk_adjustment_set_value(VHueadj, 0);

           mplayer( MPLAYER_SET_SATURATION,0,0 );
           gtk_adjustment_set_value(VSaturationadj, 0);
          }
        break;
   case 2:
        ShowEquConfig();
        break;
  }
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

  if (page_num)
  {
    gtk_widget_hide(Config);

    if (EquConfig) gtk_widget_hide(EquConfig);
  }
  else
  {
    gtk_widget_show(Config);

    if (EquConfig) gtk_widget_show(EquConfig);
  }
}

static gboolean eqDelete (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  (void) widget;
  (void) event;
  (void) user_data;

  if (memcmp(gtkEquChannels, old_gtkEquChannels, sizeof(gtkEquChannels)) != 0)
  {
    unsigned int i, j;
    equalizer_t eq;

    memcpy(gtkEquChannels, old_gtkEquChannels, sizeof(gtkEquChannels));

    for (i = 0; i < FF_ARRAY_ELEMS(gtkEquChannels); i++)
      for (j = 0; j < FF_ARRAY_ELEMS(*gtkEquChannels); j++)
      {
        eq.channel = i;
        eq.band = j;
        eq.gain = gtkEquChannels[i][j];
        mplayer(MPLAYER_SET_EQUALIZER, 0, &eq);
      }
  }

  mplayer(MPLAYER_SET_CONTRAST, vo_gamma_contrast, 0);
  mplayer(MPLAYER_SET_BRIGHTNESS, vo_gamma_brightness, 0);
  mplayer(MPLAYER_SET_HUE, vo_gamma_hue, 0);
  mplayer(MPLAYER_SET_SATURATION, vo_gamma_saturation, 0);

  eqButtonReleased(NULL, GINT_TO_POINTER(9)); // destroy windows

  return TRUE;
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
  gtk_widget_set_size_request( Equalizer,-1,256 );
  gtk_window_set_title( GTK_WINDOW( Equalizer ),_(MSGTR_GUI_Equalizer) );
  gtk_window_set_position( GTK_WINDOW( Equalizer ),GTK_WIN_POS_CENTER );
  gtk_window_set_resizable( GTK_WINDOW( Equalizer ),FALSE );
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
  gtk_widget_set_size_request( scrolledwindow1,106,-1 );
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

  memcpy(old_gtkEquChannels, gtkEquChannels, sizeof(gtkEquChannels));

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

  Channel = -1;
  eqSetBands(0);

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency0 = gtkAddLabel( _(MSGTR_GUI_Frequency0),NULL ),
    0,1,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency1 = gtkAddLabel( _(MSGTR_GUI_Frequency1),NULL ),
    1,2,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency2 = gtkAddLabel( _(MSGTR_GUI_Frequency2),NULL ),
    2,3,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency3 = gtkAddLabel( _(MSGTR_GUI_Frequency3),NULL ),
    3,4,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency4 = gtkAddLabel( _(MSGTR_GUI_Frequency4),NULL ),
    4,5,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency5 = gtkAddLabel( _(MSGTR_GUI_Frequency5),NULL ),
    5,6,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency6 = gtkAddLabel( _(MSGTR_GUI_Frequency6),NULL ),
    6,7,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency7 = gtkAddLabel( _(MSGTR_GUI_Frequency7),NULL ),
    7,8,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency8 = gtkAddLabel( _(MSGTR_GUI_Frequency8),NULL ),
    8,9,1,2,(GtkAttachOptions)( GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );

  gtk_table_attach( GTK_TABLE( table1 ),
    Frequency9 = gtkAddLabel( _(MSGTR_GUI_Frequency9),NULL ),
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

  if (vo_gamma_contrast == 1000) vo_gamma_contrast = 0;
  VContrastadj=GTK_ADJUSTMENT( gtk_adjustment_new( vo_gamma_contrast,-100,100,1,0,0 ) );
  VContrast=gtkAddHScale( VContrastadj,NULL,0 );
    gtk_table_attach( GTK_TABLE( table1 ),VContrast,1,2,0,1,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_size_request( VContrast,-1,45 );

  if (vo_gamma_brightness == 1000) vo_gamma_brightness = 0;
  VBrightnessadj=GTK_ADJUSTMENT( gtk_adjustment_new( vo_gamma_brightness,-100,100,1,0,0 ) );
  VBrightness=gtkAddHScale( VBrightnessadj,NULL,0 );
    gtk_table_attach( GTK_TABLE( table1 ),VBrightness,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_size_request( VBrightness,-1,45 );

  if (vo_gamma_hue == 1000) vo_gamma_hue = 0;
  VHueadj=GTK_ADJUSTMENT( gtk_adjustment_new( vo_gamma_hue,-100,100,1,0,0 ) );
  VHue=gtkAddHScale( VHueadj,NULL,0 );
    gtk_table_attach( GTK_TABLE( table1 ),VHue,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_size_request( VHue,-1,45 );

  if (vo_gamma_saturation == 1000) vo_gamma_saturation = 0;
  VSaturationadj=GTK_ADJUSTMENT( gtk_adjustment_new( vo_gamma_saturation,-100,100,1,0,0 ) );
  VSaturation=gtkAddHScale( VSaturationadj,NULL,0 );
    gtk_table_attach( GTK_TABLE( table1 ),VSaturation,1,2,3,4,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
    gtk_widget_set_size_request( VSaturation,-1,45 );

  gtk_notebook_set_tab_label( GTK_NOTEBOOK( Notebook ),gtk_notebook_get_nth_page( GTK_NOTEBOOK( Notebook ),1 ),
    gtkAddLabel( _(MSGTR_GUI_Video),NULL ) );

  gtkAddHSeparator( vbox1 );

  hbuttonbox1=gtkAddHButtonBox( vbox1 );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
    gtk_box_set_spacing(GTK_BOX(hbuttonbox1), 10);

  Config=gtkAddButton( _(MSGTR_GUI_Configure),hbuttonbox1 );
  Clear=gtkAddButton( _(MSGTR_GUI_Clear),hbuttonbox1 );
  Ok=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );

  gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_KEY_Escape,0,GTK_ACCEL_VISIBLE );
  gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_KEY_Return,0,GTK_ACCEL_VISIBLE );

  g_signal_connect( G_OBJECT( Equalizer ),"delete-event",G_CALLBACK( eqDelete ),NULL );
  g_signal_connect( G_OBJECT( Equalizer ),"destroy",G_CALLBACK( gtk_widget_destroyed ),&Equalizer );

  g_signal_connect( G_OBJECT( ChannelsList ),"select-row",G_CALLBACK( eqSelectChannelsListRow ),NULL );

  g_signal_connect(G_OBJECT(A3125), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(0));
  g_signal_connect(G_OBJECT(A6250), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(1));
  g_signal_connect(G_OBJECT(A125), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(2));
  g_signal_connect(G_OBJECT(A250), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(3));
  g_signal_connect(G_OBJECT(A500), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(4));
  g_signal_connect(G_OBJECT(A1000), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(5));
  g_signal_connect(G_OBJECT(A2000), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(6));
  g_signal_connect(G_OBJECT(A4000), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(7));
  g_signal_connect(G_OBJECT(A8000), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(8));
  g_signal_connect(G_OBJECT(A16000), "value-changed", G_CALLBACK(eqHScaleChanged), GINT_TO_POINTER(9));

  g_signal_connect(G_OBJECT(VContrast), "value-changed", G_CALLBACK(eqVScaleChanged), GINT_TO_POINTER(1));
  g_signal_connect(G_OBJECT(VBrightness), "value-changed", G_CALLBACK(eqVScaleChanged), GINT_TO_POINTER(2));
  g_signal_connect(G_OBJECT(VHue), "value-changed", G_CALLBACK(eqVScaleChanged), GINT_TO_POINTER(3));
  g_signal_connect(G_OBJECT(VSaturation), "value-changed", G_CALLBACK(eqVScaleChanged), GINT_TO_POINTER(4));

  g_signal_connect( G_OBJECT( Ok ),"clicked",G_CALLBACK( eqButtonReleased ),GINT_TO_POINTER(0) );
  g_signal_connect( G_OBJECT( Clear ),"clicked",G_CALLBACK( eqButtonReleased ),GINT_TO_POINTER(1) );
  g_signal_connect( G_OBJECT( Config ),"clicked",G_CALLBACK( eqButtonReleased ),GINT_TO_POINTER(2) );

  g_signal_connect( G_OBJECT( Notebook ),"switch-page",G_CALLBACK( eqNotebook ),NULL );

  gtk_window_add_accel_group( GTK_WINDOW( Equalizer ),accel_group );

  return Equalizer;
}

void ShowEqualizer( void )
{
 gboolean set;

 if ( Equalizer ) gtkRaise( Equalizer );
    else Equalizer=CreateEqualizer();

 if ( !gtkEquChannel1 ) gtkEquChannel1=strdup( _(MSGTR_GUI_FrontRight) );
 if ( !gtkEquChannel2 ) gtkEquChannel2=strdup( _(MSGTR_GUI_FrontLeft) );
 if ( !gtkEquChannel3 ) gtkEquChannel3=strdup( _(MSGTR_GUI_RearRight) );
 if ( !gtkEquChannel4 ) gtkEquChannel4=strdup( _(MSGTR_GUI_RearLeft) );
 if ( !gtkEquChannel5 ) gtkEquChannel5=strdup( _(MSGTR_GUI_Center) );
 if ( !gtkEquChannel6 ) gtkEquChannel6=strdup( _(MSGTR_GUI_Bass) );

 eqSetChannelNames();

 set = (guiInfo.Playing && guiInfo.sh_video);

 gtk_widget_set_sensitive(VContrast, set);
 gtk_widget_set_sensitive(VBrightness, set);
 gtk_widget_set_sensitive(VHue, set);
 gtk_widget_set_sensitive(VSaturation, set);

 set = (guiInfo.Playing && gtkEnableAudioEqualizer);

 gtk_widget_set_sensitive(ChannelsList, set);
 gtk_widget_set_sensitive(A3125, set);
 gtk_widget_set_sensitive(A125, set);
 gtk_widget_set_sensitive(A6250, set);
 gtk_widget_set_sensitive(A250, set);
 gtk_widget_set_sensitive(A500, set);
 gtk_widget_set_sensitive(A1000, set);
 gtk_widget_set_sensitive(A2000, set);
 gtk_widget_set_sensitive(A4000, set);
 gtk_widget_set_sensitive(A8000, set);
 gtk_widget_set_sensitive(A16000, set);
 gtk_widget_set_sensitive(Frequency0, set);
 gtk_widget_set_sensitive(Frequency1, set);
 gtk_widget_set_sensitive(Frequency2, set);
 gtk_widget_set_sensitive(Frequency3, set);
 gtk_widget_set_sensitive(Frequency4, set);
 gtk_widget_set_sensitive(Frequency5, set);
 gtk_widget_set_sensitive(Frequency6, set);
 gtk_widget_set_sensitive(Frequency7, set);
 gtk_widget_set_sensitive(Frequency8, set);
 gtk_widget_set_sensitive(Frequency9, set);

 gtk_widget_show( Equalizer );
}

/* equalizer config dialog box */

static GtkWidget * CBChannel1;
static GtkWidget * CBChannel2;
static GtkWidget * CBChannel3;
static GtkWidget * CBChannel4;
static GtkWidget * CBChannel5;
static GtkWidget * CBChannel6;
static GtkWidget * ecOk;
static GtkWidget * ecCancel;

GtkWidget * CreateEquConfig( void );

void ShowEquConfig( void )
{
 GSList *Items = NULL, *list;

 if (EquConfig)
 {
   gtkSetLayer(EquConfig);
   return;
 }
 else EquConfig = CreateEquConfig();

 Items = g_slist_append(Items, _(MSGTR_GUI_FrontRight));
 Items = g_slist_append(Items, _(MSGTR_GUI_FrontLeft));
 Items = g_slist_append(Items, _(MSGTR_GUI_RearRight));
 Items = g_slist_append(Items, _(MSGTR_GUI_RearLeft));
 Items = g_slist_append(Items, _(MSGTR_GUI_Center));
 Items = g_slist_append(Items, _(MSGTR_GUI_Bass));

 list = Items;

 while (list)
 {
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel1), list->data);
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel2), list->data);
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel3), list->data);
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel4), list->data);
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel5), list->data);
   gtk_combo_box_append_text(GTK_COMBO_BOX(CBChannel6), list->data);

   list = list->next;
 }

 g_slist_free(Items);

 gtk_entry_set_text(gtkEntry(CBChannel1), gtkEquChannel1);
 gtk_entry_set_text(gtkEntry(CBChannel2), gtkEquChannel2);
 gtk_entry_set_text(gtkEntry(CBChannel3), gtkEquChannel3);
 gtk_entry_set_text(gtkEntry(CBChannel4), gtkEquChannel4);
 gtk_entry_set_text(gtkEntry(CBChannel5), gtkEquChannel5);
 gtk_entry_set_text(gtkEntry(CBChannel6), gtkEquChannel6);

 gtk_widget_show( EquConfig );
 gtkSetLayer( EquConfig );
}

static void ecButtonReleased( GtkButton * button,gpointer user_data )
{
 (void) button;

 if ( GPOINTER_TO_INT(user_data) )
 { // if you pressed Ok
  nfree( gtkEquChannel1 ); gtkEquChannel1=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel1 ) ) );
  nfree( gtkEquChannel2 ); gtkEquChannel2=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel2 ) ) );
  nfree( gtkEquChannel3 ); gtkEquChannel3=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel3 ) ) );
  nfree( gtkEquChannel4 ); gtkEquChannel4=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel4 ) ) );
  nfree( gtkEquChannel5 ); gtkEquChannel5=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel5 ) ) );
  nfree( gtkEquChannel6 ); gtkEquChannel6=gstrdup( gtk_entry_get_text( gtkEntry( CBChannel6 ) ) );
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
  gtk_widget_set_size_request( EquConfig,350,260 );
  gtk_window_set_title( GTK_WINDOW( EquConfig ),_(MSGTR_GUI_EqualizerConfiguration) );
  gtk_window_set_position( GTK_WINDOW( EquConfig ),GTK_WIN_POS_CENTER );
  gtk_window_set_resizable( GTK_WINDOW( EquConfig ),FALSE );
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
  gtkEntrySetEditable(CBChannel1, FALSE);

  CBChannel2=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel2,1,2,1,2,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  gtkEntrySetEditable(CBChannel2, FALSE);

  CBChannel3=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel3,1,2,2,3,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  gtkEntrySetEditable(CBChannel3, FALSE);

  CBChannel4=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel4,1,2,3,4,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  gtkEntrySetEditable(CBChannel4, FALSE);

  CBChannel5=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel5,1,2,4,5,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  gtkEntrySetEditable(CBChannel5, FALSE);

  CBChannel6=gtkAddCombo( NULL );
    gtk_table_attach( GTK_TABLE( table1 ),CBChannel6,1,2,5,6,(GtkAttachOptions)( GTK_EXPAND | GTK_FILL ),(GtkAttachOptions)( 0 ),0,0 );
  gtkEntrySetEditable(CBChannel6, FALSE);

  gtkAddHSeparator( vbox1 );

  hbuttonbox1=gtkAddHButtonBox( vbox1 );
    gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox1 ),GTK_BUTTONBOX_END );
    gtk_box_set_spacing(GTK_BOX(hbuttonbox1), 10);

  ecOk=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox1 );
  ecCancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox1 );

  gtk_widget_add_accelerator( ecOk,"clicked",accel_group,GDK_KEY_Return,0,GTK_ACCEL_VISIBLE );
  gtk_widget_add_accelerator( ecCancel,"clicked",accel_group,GDK_KEY_Escape,0,GTK_ACCEL_VISIBLE );

  g_signal_connect( G_OBJECT( EquConfig ),"destroy",G_CALLBACK( gtk_widget_destroyed ),&EquConfig );

  g_signal_connect( G_OBJECT( ecOk ),"clicked",G_CALLBACK( ecButtonReleased ),GINT_TO_POINTER(1) );
  g_signal_connect( G_OBJECT( ecCancel ),"clicked",G_CALLBACK( ecButtonReleased ),GINT_TO_POINTER(0) );

  gtk_window_add_accel_group( GTK_WINDOW( EquConfig ),accel_group );

  return EquConfig;
}
