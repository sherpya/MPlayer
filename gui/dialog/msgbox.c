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

#include <string.h>

#include "gui/app/app.h"
#include "gui/app/gui.h"
#include "help_mp.h"

#include "dialog.h"
#include "icons.h"
#include "msgbox.h"
#include "tools.h"

GtkWidget * gtkMessageBoxText;
GtkWidget * MessageBox = NULL;
GtkWidget * InformationImage;
GtkWidget * WarningImage;
GtkWidget * ErrorImage;

static void on_Ok_released( GtkButton * button,gpointer user_data  )
{
 (void) button;
 (void) user_data;

 gtk_widget_destroy( MessageBox );
}

static GtkWidget * CreateMessageBox( void )
{
 GtkWidget * vbox1;
 GtkWidget * hbox1;
 GtkWidget * hbuttonbox1;
 GtkWidget * Ok;
 GtkAccelGroup * accel_group;
 GdkPixbuf * pixbuf;

 accel_group=gtk_accel_group_new();

 MessageBox=gtk_window_new( GTK_WINDOW_TOPLEVEL );
 gtk_widget_set_events( MessageBox,GDK_EXPOSURE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_VISIBILITY_NOTIFY_MASK );
 gtk_window_set_title( GTK_WINDOW( MessageBox ),"MPlayer " ELLIPSIS );
 gtk_window_set_position( GTK_WINDOW( MessageBox ),GTK_WIN_POS_CENTER );
 gtk_window_set_modal( GTK_WINDOW( MessageBox ),TRUE );
 gtk_window_set_policy( GTK_WINDOW( MessageBox ),FALSE,FALSE,TRUE );
 gtk_window_set_wmclass( GTK_WINDOW( MessageBox ),"Message",MPlayer );

 gtk_widget_realize( MessageBox );
 gtkAddIcon( MessageBox );

 vbox1=gtkAddVBox( gtkAddDialogFrame( MessageBox ),0 );
 hbox1=gtkAddHBox( vbox1,1 );

 pixbuf = gdk_pixbuf_new_from_inline(-1, information_png, FALSE, NULL);
 InformationImage = gtk_image_new_from_pixbuf(pixbuf);
 g_object_unref(pixbuf);

 gtk_widget_hide( InformationImage );
 gtk_box_pack_start( GTK_BOX( hbox1 ),InformationImage,FALSE,FALSE,2 );

 pixbuf = gdk_pixbuf_new_from_inline(-1, warning_png, FALSE, NULL);
 WarningImage = gtk_image_new_from_pixbuf(pixbuf);
 g_object_unref(pixbuf);

 gtk_widget_hide( WarningImage );
 gtk_box_pack_start( GTK_BOX( hbox1 ),WarningImage,FALSE,FALSE,2 );

 pixbuf = gdk_pixbuf_new_from_inline(-1, error_png, FALSE, NULL);
 ErrorImage = gtk_image_new_from_pixbuf(pixbuf);
 g_object_unref(pixbuf);

 gtk_widget_hide( ErrorImage );
 gtk_box_pack_start( GTK_BOX( hbox1 ),ErrorImage,FALSE,FALSE,2 );

 gtkMessageBoxText=gtk_label_new( "Text jol. Ha ezt megerted,akkor neked nagyon jo a magyar tudasod,te." );
 gtk_widget_show( gtkMessageBoxText );
 gtk_box_pack_start( GTK_BOX( hbox1 ),gtkMessageBoxText,FALSE,FALSE,4 );
// gtk_label_set_justify( GTK_LABEL( gtkMessageBoxText ),GTK_JUSTIFY_FILL );
 gtk_label_set_justify( GTK_LABEL( gtkMessageBoxText ),GTK_JUSTIFY_CENTER );
 gtk_label_set_line_wrap( GTK_LABEL( gtkMessageBoxText ),FALSE );

 gtkAddHSeparator( vbox1 );
 hbuttonbox1=gtkAddHButtonBox( vbox1 );
 Ok=gtkAddButton( MSGTR_GUI_Ok,hbuttonbox1 );

 gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );
 gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );

 gtk_signal_connect( GTK_OBJECT( MessageBox ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&MessageBox );
 gtk_signal_connect( GTK_OBJECT( Ok ),"clicked",GTK_SIGNAL_FUNC( on_Ok_released ),NULL );

 gtk_window_add_accel_group( GTK_WINDOW( MessageBox ),accel_group );

 return MessageBox;
}

void ShowMessageBox( const char * msg )
{
 (void) msg;

 if ( MessageBox ) gtk_widget_destroy( MessageBox );
 MessageBox=CreateMessageBox();
}
