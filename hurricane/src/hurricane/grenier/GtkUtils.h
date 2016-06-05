// ****************************************************************************************************
// File: GtkUtils.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_GTK_UTILS
#define HURRICANE_GTK_UTILS

#include "Commons.h"

#include <gtk/gtk.h>

#define GTK_FILL_EXPAND ((GtkAttachOptions)(GTK_FILL | GTK_EXPAND))

namespace Hurricane {



// ****************************************************************************************************
// Globals
// ****************************************************************************************************

void gtk_check_for_interruption();

GdkWindow* gtk_get_window();

GdkColor* gdk_color_new(unsigned short red, unsigned short green, unsigned short blue);
GdkGC* gtk_gc_new(GdkColor* color, const string& pattern = "FFFFFFFFFFFFFFFF", unsigned linewidth=0);
GdkGC* gtk_gc_new(unsigned short red, unsigned short green, unsigned short blue, const string& pattern = "FFFFFFFFFFFFFFFF", unsigned linewidth=0);

GtkWidget* gtk_pixmap_new_from_name(gchar* pixmapName[]);

GtkWidget* gtk_button_new_with_pixmap(gchar* pixmapName[]);

GtkWidget* gtk_hbutton_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString);
GtkWidget* gtk_vbutton_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString);

GtkWidget* gtk_menu_item_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString);

string  GdkKeyvalName ( int keyval );


} // End of Hurricane namespace.

#endif // HURRICANE_GTK_UTILS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
