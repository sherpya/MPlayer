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

#include <glob.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "skinbrowser.h"
#include "dialog.h"
#include "tools.h"
#include "gui/gtk-compat.h"
#include "gui/app/cfg.h"
#include "gui/app/gui.h"
#include "gui/skin/skin.h"
#include "gui/ui/actions.h"

#include "help_mp.h"

GtkWidget *SkinBrowser;

static GtkWidget *SkinList;
static char *currSelected;
static char *prevSelected;

static void FillSkinList (char *dir)
{
 char          * pattern;
 size_t          i;
 glob_t          gg;
 struct stat     fs;
 gchar         * name[1];

 if ((pattern = calloc(1, strlen(dir) + 4)))
 {
   strcpy(pattern, dir);
   strcat(pattern, "/*");
 }
 else return;

 glob( pattern,GLOB_NOSORT,NULL,&gg );
 for( i=0;i<gg.gl_pathc;i++ )
  {
   if ( !strcmp( gg.gl_pathv[i],"." ) || !strcmp( gg.gl_pathv[i],".." ) ) continue;
   if ( ( lstat( gg.gl_pathv[i],&fs ) == 0 ) )
    {
     dir=strrchr( gg.gl_pathv[i],'/' );
     if (dir) dir++;
     else dir = gg.gl_pathv[i];
     // only directories or a link named "default"
     if ( S_ISDIR( fs.st_mode ) || ( ( fs.st_mode & S_IFMT ) == S_IFLNK  && strcmp( dir,"default" ) == 0 ) )
      {
       name[0]=dir;
       if ( gtkFindInCList( SkinList,name[0] ) == -1 ) gtk_clist_append( GTK_CLIST( SkinList ),name );
      }
    }
  }
 globfree( &gg );
 free(pattern);
}

static void select_row (GtkCList *clist, gint row, gint column, GdkEvent *event, gpointer user_data)
{
 (void) column;
 (void) user_data;

 gtk_clist_get_text( clist,row,0,&currSelected );
 if ( strcmp( prevSelected,currSelected ) )
  {
   prevSelected=currSelected;
   uiChangeSkin( currSelected );
   gtk_widget_hide( SkinBrowser );
   gtk_widget_show( SkinBrowser );
  }
 if( !event ) return;
 if( event->type == GDK_2BUTTON_PRESS )
  {
   free( skinName );
   skinName=strdup( currSelected );
   gtk_widget_destroy( SkinBrowser );
  }
}

static void button_clicked (GtkButton *button, gpointer user_data)
{
 (void) button;

 if ( currSelected )
 {
  switch ( GPOINTER_TO_INT(user_data) )
   {
    case 0: // cancel
      if ( strcmp( currSelected,skinName ) ) uiChangeSkin( skinName );
      break;
   case 1: // ok
      free( skinName );
      skinName=strdup( currSelected );
      break;
  }
 }
 gtk_widget_destroy( SkinBrowser );
}

static gboolean window_delete (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  (void) widget;
  (void) event;
  (void) user_data;

  button_clicked(NULL, 0); // press cancel and destroy window

  return TRUE;
}

static GtkWidget *CreateSkinBrowser (void)
{
 GtkWidget     * vbox;
 GtkWidget     * scrolledwindow;
 GtkWidget     * hbuttonbox;
 GtkWidget     * Ok;
 GtkWidget     * Cancel;
 GtkAccelGroup * accel_group;

 accel_group = gtk_accel_group_new ();

 SkinBrowser=gtk_window_new( GTK_WINDOW_TOPLEVEL );
 gtk_widget_set_size_request( SkinBrowser,256,320 );
 gtk_container_set_border_width( GTK_CONTAINER( SkinBrowser ),1 );
 gtk_window_set_title( GTK_WINDOW( SkinBrowser ),_(MSGTR_GUI_SkinBrowser) );
 gtk_window_set_position( GTK_WINDOW( SkinBrowser ),GTK_WIN_POS_CENTER );
 gtk_window_set_resizable( GTK_WINDOW( SkinBrowser ),FALSE );
 gtk_window_set_wmclass( GTK_WINDOW( SkinBrowser ),"SkinBrowser",MPlayer );

 gtk_widget_realize( SkinBrowser );
 gtkAddIcon( SkinBrowser );

 vbox=gtkAddVBox( gtkAddDialogFrame( SkinBrowser ),0 );

 scrolledwindow=gtk_scrolled_window_new( NULL,NULL );
 g_object_ref( scrolledwindow );
 g_object_set_data_full( G_OBJECT( SkinBrowser ),"sbScrolledWindow",scrolledwindow,(GDestroyNotify)g_object_unref );
 gtk_widget_show( scrolledwindow );
 gtk_box_pack_start( GTK_BOX( vbox ),scrolledwindow,TRUE,TRUE,0 );
 gtk_container_set_border_width( GTK_CONTAINER( scrolledwindow ),2 );
 gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( scrolledwindow ),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC );

 SkinList=gtk_clist_new( 1 );
 g_object_ref( SkinList );
 g_object_set_data_full( G_OBJECT( SkinBrowser ),"SkinList",SkinList,(GDestroyNotify)g_object_unref );
 gtk_widget_show( SkinList );
 gtk_container_add( GTK_CONTAINER( scrolledwindow ),SkinList );
 gtk_clist_set_column_width( GTK_CLIST( SkinList ),0,80 );
 gtk_clist_set_selection_mode( GTK_CLIST( SkinList ),GTK_SELECTION_SINGLE );
 gtk_clist_column_titles_show( GTK_CLIST( SkinList ) );
 gtk_clist_set_shadow_type( GTK_CLIST( SkinList ),GTK_SHADOW_ETCHED_OUT );
 gtk_clist_set_column_widget( GTK_CLIST( SkinList ),0, gtkAddLabel( _(MSGTR_GUI_AvailableSkins),NULL ) );
 gtk_clist_column_title_passive( GTK_CLIST( SkinList ),0 );

 gtkAddHSeparator( vbox );

 hbuttonbox=gtkAddHButtonBox( vbox );
  gtk_button_box_set_layout( GTK_BUTTON_BOX( hbuttonbox ),GTK_BUTTONBOX_SPREAD );
 gtk_box_set_spacing(GTK_BOX(hbuttonbox), 10);

 Ok=gtkAddButton( _(MSGTR_GUI_Ok),hbuttonbox );
 Cancel=gtkAddButton( _(MSGTR_GUI_Cancel),hbuttonbox );

 gtk_widget_add_accelerator( Ok,"clicked",accel_group,GDK_KEY_Return,0,GTK_ACCEL_VISIBLE );
 gtk_widget_add_accelerator( Cancel,"clicked",accel_group,GDK_KEY_Escape,0,GTK_ACCEL_VISIBLE );

 g_signal_connect( G_OBJECT( SkinBrowser ),"delete-event",G_CALLBACK( window_delete ),NULL );
 g_signal_connect( G_OBJECT( SkinBrowser ),"destroy",G_CALLBACK( gtk_widget_destroyed ),&SkinBrowser );
 g_signal_connect( G_OBJECT( SkinList ),"select-row",G_CALLBACK( select_row ),NULL );
 g_signal_connect( G_OBJECT( Ok ),"clicked",G_CALLBACK( button_clicked ),GINT_TO_POINTER(1) );
 g_signal_connect( G_OBJECT( Cancel ),"clicked",G_CALLBACK( button_clicked ),GINT_TO_POINTER(0) );

 gtk_window_add_accel_group( GTK_WINDOW( SkinBrowser ),accel_group );
 gtk_widget_grab_focus( scrolledwindow );

 return SkinBrowser;
}

void ShowSkinBrowser (void)
{
  gint i;

  if (SkinBrowser)
  {
    gtkRaise(SkinBrowser);
    return;
  }
  else SkinBrowser = CreateSkinBrowser();

  currSelected = NULL;
  prevSelected = skinName;

  FillSkinList(skinDirInHome);
  FillSkinList(skinDirInData);

  if ((i = gtkFindInCList(SkinList, skinName)) > -1)
    gtk_clist_select_row(GTK_CLIST(SkinList), i, 0);

  gtk_clist_sort(GTK_CLIST(SkinList));

  gtk_widget_show(SkinBrowser);
}
