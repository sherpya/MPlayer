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

#include <sys/stat.h>
#include <glob.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "skinbrowser.h"
#include "tools.h"

#include "gui/app/app.h"
#include "gui/app/cfg.h"
#include "gui/app/gui.h"
#include "gui/ui/actions.h"
#include "gui/skin/skin.h"
#include "help_mp.h"

#include "dialog.h"

GtkWidget * SkinList = NULL;
char      * sbSelectedSkin=NULL;
char      * sbSkinDirInHome=NULL;
char      * sbSkinDirInData=NULL;

static char *prevSelected;

GtkWidget * SkinBrowser = NULL;

static void prButton( GtkButton * button,gpointer user_data )
{
 (void) button;

 if ( sbSelectedSkin )
 {
  switch ( GPOINTER_TO_INT(user_data) )
   {
    case 0: // cancel
      if ( strcmp( sbSelectedSkin,skinName ) ) uiChangeSkin( skinName );
      break;
   case 1: // ok
      free( skinName );
      skinName=strdup( sbSelectedSkin );
      break;
  }
 }
 gtk_widget_destroy( SkinBrowser );
}

static void on_SkinList_select_row( GtkCList * clist,gint row,gint column,GdkEvent * event,gpointer user_data )
{
 (void) column;
 (void) user_data;

 gtk_clist_get_text( clist,row,0,&sbSelectedSkin );
 if ( strcmp( prevSelected,sbSelectedSkin ) )
  {
   prevSelected=sbSelectedSkin;
   uiChangeSkin( sbSelectedSkin );
   gtk_widget_hide( SkinBrowser );
   gtk_widget_show( SkinBrowser );
  }
 if( !event ) return;
 if( event->type == GDK_2BUTTON_PRESS )
  {
   free( skinName );
   skinName=strdup( sbSelectedSkin );
   gtk_widget_destroy( SkinBrowser );
  }
}

void ShowSkinBrowser( void )
{
 GtkWidget     * vbox5;
 GtkWidget     * scrolledwindow1;
 GtkWidget     * hbuttonbox4;
 GtkWidget     * Cancel;
 GtkWidget     * Ok;
 GtkAccelGroup * accel_group;

 accel_group = gtk_accel_group_new ();

 SkinBrowser=gtk_window_new( GTK_WINDOW_TOPLEVEL );
 gtk_widget_set_usize( SkinBrowser,256,320 );
 gtk_container_set_border_width( GTK_CONTAINER( SkinBrowser ),1 );
 gtk_window_set_title( GTK_WINDOW( SkinBrowser ),_(MSGTR_GUI_SkinBrowser) );
 gtk_window_set_position( GTK_WINDOW( SkinBrowser ),GTK_WIN_POS_CENTER );
 gtk_window_set_policy( GTK_WINDOW( SkinBrowser ),FALSE,FALSE,TRUE );
 gtk_window_set_wmclass( GTK_WINDOW( SkinBrowser ),"SkinBrowser",MPlayer );

 gtk_widget_realize( SkinBrowser );
 gtkAddIcon( SkinBrowser );

 vbox5=gtkAddVBox( gtkAddDialogFrame( SkinBrowser ),0 );

 scrolledwindow1=gtk_scrolled_window_new( NULL,NULL );
 gtk_widget_ref( scrolledwindow1 );
 gtk_object_set_data_full( GTK_OBJECT( SkinBrowser ),"scrolledwindow1",scrolledwindow1,(GtkDestroyNotify)gtk_widget_unref );
 gtk_widget_show( scrolledwindow1 );
 gtk_box_pack_start( GTK_BOX( vbox5 ),scrolledwindow1,TRUE,TRUE,0 );
 gtk_container_set_border_width( GTK_CONTAINER( scrolledwindow1 ),2 );
 gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolledwindow1 ),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC );

 SkinList=gtk_clist_new( 1 );
 gtk_widget_ref( SkinList );
 gtk_object_set_data_full( GTK_OBJECT( SkinBrowser ),"SkinList",SkinList,(GtkDestroyNotify)gtk_widget_unref );
 gtk_widget_show( SkinList );
 gtk_container_add( GTK_CONTAINER( scrolledwindow1 ),SkinList );
 gtk_clist_set_column_width( GTK_CLIST( SkinList ),0,80 );
 gtk_clist_set_selection_mode( GTK_CLIST( SkinList ),GTK_SELECTION_SINGLE );
 gtk_clist_column_titles_show( GTK_CLIST( SkinList ) );
 gtk_clist_set_shadow_type( GTK_CLIST( SkinList ),GTK_SHADOW_ETCHED_OUT );
 gtk_clist_set_column_widget( GTK_CLIST( SkinList ),0, gtkAddLabel( _(MSGTR_GUI_AvailableSkins),NULL ) );
 gtk_clist_column_title_passive( GTK_CLIST( SkinList ),0 );

 gtkAddHSeparator( vbox5 );

 hbuttonbox4=gtkAddHButtonBox( vbox5 );
  gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox4 ),GTK_BUTTONBOX_SPREAD );
  gtk_button_box_set_spacing( GTK_BUTTON_BOX( hbuttonbox4 ),10 );

 Ok=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox4 );
 Cancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox4 );

 gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_Return,0,GTK_ACCEL_VISIBLE );
 gtk_widget_add_accelerator( Cancel,"clicked",accel_group,GDK_Escape,0,GTK_ACCEL_VISIBLE );

 gtk_signal_connect( GTK_OBJECT( SkinBrowser ),"destroy",GTK_SIGNAL_FUNC( gtk_widget_destroyed ),&SkinBrowser );
 gtk_signal_connect( GTK_OBJECT( SkinList ),"select-row",GTK_SIGNAL_FUNC( on_SkinList_select_row ),NULL );
 gtk_signal_connect( GTK_OBJECT( Ok ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(1) );
 gtk_signal_connect( GTK_OBJECT( Cancel ),"clicked",GTK_SIGNAL_FUNC( prButton ),GINT_TO_POINTER(0) );

 if ( ( sbSkinDirInHome=calloc( 1,strlen( skinDirInHome ) + 4 ) ) != NULL )
  { strcpy( sbSkinDirInHome,skinDirInHome ); strcat( sbSkinDirInHome,"/*" ); }
 if ( ( sbSkinDirInData=calloc( 1,strlen( skinDirInData ) + 4 ) ) != NULL )
  { strcpy( sbSkinDirInData,skinDirInData ); strcat( sbSkinDirInData,"/*" ); }

 gtk_window_add_accel_group( GTK_WINDOW( SkinBrowser ),accel_group );
 gtk_widget_grab_focus( scrolledwindow1 );

 prevSelected = skinName;
 gtk_widget_show(SkinBrowser);
}

int FillSkinList( gchar * mdir )
{
 gchar         * str[1];
 gchar         * tmp;
 size_t          i;
 glob_t          gg;
 struct stat     fs;

 glob( mdir,GLOB_NOSORT,NULL,&gg );
 for( i=0;i<gg.gl_pathc;i++ )
  {
   if ( !strcmp( gg.gl_pathv[i],"." ) || !strcmp( gg.gl_pathv[i],".." ) ) continue;
   if ( ( lstat( gg.gl_pathv[i],&fs ) == 0 ) )
    {
     tmp=strrchr( gg.gl_pathv[i],'/' );
     if (tmp) tmp++;
     else tmp = gg.gl_pathv[i];
     // only directories or a link named "default"
     if ( S_ISDIR( fs.st_mode ) || ( ( fs.st_mode & S_IFMT ) == S_IFLNK  && strcmp( tmp,"default" ) == 0 ) )
      {
       str[0]=tmp;
       if ( gtkFindInCList( SkinList,str[0] ) == -1 ) gtk_clist_append( GTK_CLIST( SkinList ),str );
      }
    }
  }
 globfree( &gg );
 return True;
}
