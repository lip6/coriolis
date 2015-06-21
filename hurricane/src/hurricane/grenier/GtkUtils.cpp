// ****************************************************************************************************
// File: GtkUtils.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#include "GtkUtils.h"
#include "Interruption.h"

namespace Hurricane {




// ****************************************************************************************************
// Globals
// ****************************************************************************************************

static bool gtk_has_been_aborted()
// *******************************
{
	bool aborted = false;
	if (gdk_events_pending()) {
		GdkEvent* event = gdk_event_get();
		if (event) {
			if ((event->type == GDK_KEY_PRESS) &&
				 (string(gdk_keyval_name(((GdkEventKey*)event)->keyval)) == "Escape"))
				aborted = true;
			else
				aborted = gtk_has_been_aborted();
			gdk_event_put(event);
			gdk_event_free(event);
		}
	}
	return aborted;
}

void gtk_check_for_interruption()
// ******************************
{
	if (gtk_has_been_aborted()) throw Interruption("Escape");
}

GdkWindow* gtk_get_window()
// ************************
{
	static GdkWindow* WINDOW = (GdkWindow*)NULL;
	if (!WINDOW) {
		GtkWidget* widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_widget_realize(widget);
		WINDOW = widget->window;
	}
	return WINDOW;
}

GdkColor* gdk_color_new(unsigned short red, unsigned short green, unsigned short blue)
// ***********************************************************************************
{
  GdkWindow* window = gtk_get_window();
  GdkColor* color = new GdkColor;
  color->red = red * 255;
  color->green = green * 255;
  color->blue = blue * 255;

  gdk_color_alloc(gdk_window_get_colormap(window), color);

  return color;
}

GdkGC* gtk_gc_new(GdkColor* color, const string& pattern, unsigned linewidth)
// **************************************************************************
{
	GdkWindow* window = gtk_get_window();
	gdk_color_alloc(gdk_window_get_colormap(window), color);
	GdkGC* gc = gdk_gc_new(window);
	gdk_gc_set_foreground(gc, color);

        if (linewidth != 0)
        {
            gdk_gc_set_line_attributes(gc
                    , linewidth
                    , GDK_LINE_SOLID 
                    , GDK_CAP_NOT_LAST
                    , GDK_JOIN_MITER );
        }

	if (pattern == "FFFFFFFFFFFFFFFF") {
		gdk_gc_set_fill(gc, GDK_SOLID);
	}
	else {
		gdk_gc_set_fill(gc, GDK_STIPPLED);
		char bits[8];
		for (int i = 0; i < 8; i++) {
			int high = pattern[i * 2];
			if (('0' <= high) && (high <= '9'))
				high = high - '0';
			else {
				if (('a' <= high) && (high <= 'f'))
					high = 10 + high - 'a';
				else {
					if (('A' <= high) && (high <= 'F'))
						high = 10 + high - 'A';
					else
						high = '0';
				}
			}
			int low = pattern[(i * 2) + 1];
			if (('0' <= low) && (low <= '9'))
				low = low - '0';
			else {
				if (('a' <= low) && (low <= 'f'))
					low = 10 + low - 'a';
				else {
					if (('A' <= low) && (low <= 'F'))
						low = 10 + low - 'A';
					else
						low = '0';
				}
			}
			bits[i] = (char)((high * 16) + low);
		}
		GdkPixmap* stipple = gdk_bitmap_create_from_data(window, bits, 8, 8);
		gdk_gc_set_stipple(gc, stipple);
	}
	return gc;
}

GdkGC* gtk_gc_new(unsigned short red, unsigned short green, unsigned short blue, const string& pattern, unsigned linewidth)
// ****************************************************************************************************
{
	GdkColor color;
	color.red = red * 255;
	color.green = green * 255;
	color.blue = blue * 255;

    return gtk_gc_new(&color, pattern, linewidth);
}

GtkWidget* gtk_pixmap_new_from_name(gchar* pixmapName[])
// *****************************************************
{
	GdkBitmap* mask;
	GdkPixmap* pixmap =
		gdk_pixmap_create_from_xpm_d(gtk_get_window(),
							 				  &mask,
											  (GdkColor*)NULL,
											  pixmapName);
	GtkWidget* widget = gtk_pixmap_new(pixmap, mask);
	gdk_pixmap_unref(pixmap);
	gdk_pixmap_unref(mask);

	return widget;
}

GtkWidget* gtk_button_new_with_pixmap(gchar* pixmapName[])
// *******************************************************
{
	GtkWidget* button = gtk_button_new();

	GtkWidget* pixmap = gtk_pixmap_new_from_name(pixmapName);

	gtk_container_add(GTK_CONTAINER(button), pixmap);

	return button;
}

GtkWidget* gtk_hbutton_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString)
// **************************************************************************************
{
	GtkWidget* button = gtk_button_new();

	GtkWidget* table = gtk_table_new(4, 1, false);
	gtk_container_add(GTK_CONTAINER(button), table);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);

	gtk_table_attach
		(GTK_TABLE(table), gtk_label_new(""), 0, 1, 0, 1, GTK_FILL_EXPAND, GTK_FILL, 0, 0);

	GtkWidget* pixmap = gtk_pixmap_new_from_name(pixmapName);
	gtk_table_attach
		(GTK_TABLE(table), pixmap, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	GtkWidget* label = gtk_label_new(labelString);
	gtk_table_attach
		(GTK_TABLE(table), label, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

	gtk_table_attach
		(GTK_TABLE(table), gtk_label_new(""), 3, 4, 0, 1, GTK_FILL_EXPAND, GTK_FILL, 0, 0);

	gtk_table_set_col_spacing(GTK_TABLE(table), 2, 11);

	return button;
}

GtkWidget* gtk_vbutton_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString)
// **************************************************************************************
{
	GtkWidget* button = gtk_button_new();

	GtkWidget* table = gtk_table_new(1, 4, false);
	gtk_container_add(GTK_CONTAINER(button), table);
	gtk_table_set_row_spacings(GTK_TABLE(table), 2);

	GtkWidget* pixmap = gtk_pixmap_new_from_name(pixmapName);
	gtk_table_attach
		(GTK_TABLE(table), pixmap, 0, 1, 1, 2, GTK_FILL_EXPAND, GTK_FILL_EXPAND, 0, 0);

	GtkWidget* label = gtk_label_new(labelString);
	gtk_table_attach
		(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_FILL_EXPAND, GTK_FILL_EXPAND, 0, 0);

	return button;
}

GtkWidget* gtk_menu_item_new_with_pixmap_and_label(gchar* pixmapName[], gchar* labelString)
// ****************************************************************************************
{
	GtkWidget* menuItem = gtk_menu_item_new();

	GtkWidget* hBox = gtk_hbox_new(FALSE, 0);

	GtkWidget* pixmap = gtk_pixmap_new_from_name(pixmapName);
	gtk_box_pack_start(GTK_BOX(hBox), pixmap, FALSE, FALSE, 5);

	GtkWidget* label = gtk_label_new(labelString);
	gtk_box_pack_start(GTK_BOX(hBox), label, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(menuItem), hBox);

	return menuItem;
}




// ---------------------------------------------------------------
// Function  :  "GdkKeyvalName()".

string  GdkKeyvalName ( int keyval )
{
  string  skeyval = "None";
  if ( keyval != 0 ) skeyval = gdk_keyval_name(keyval);

  return ( skeyval );
}




} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
