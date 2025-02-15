/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Marco tab popup window */

/*
 * Copyright (C) 2001 Havoc Pennington
 * Copyright (C) 2005 Elijah Newren
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef META_TABPOPUP_H
#define META_TABPOPUP_H

/* Don't include gtk.h or gdk.h here */
#include <X11/Xlib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "boxes.h"
#include "common.h"

typedef struct _MetaTabEntry MetaTabEntry;
typedef struct _MetaTabPopup MetaTabPopup;
typedef void *MetaTabEntryKey;

typedef enum border_outline_t {
  BORDER_OUTLINE_WINDOW = 1,
  BORDER_OUTLINE_TAB = 2,
  BORDER_OUTLINE_WORKSPACE = 4
} border_outline_t;

struct _MetaTabEntry {
  MetaTabEntryKey key;
  const char *title;
  GdkPixbuf *icon;
  cairo_surface_t *win_surface;
  MetaRectangle rect;
  MetaRectangle inner_rect;
  guint blank : 1;
  guint hidden : 1;
  guint demands_attention : 1;
};

MetaTabPopup *meta_ui_tab_popup_new(const MetaTabEntry *entries,
                                    int entry_count, int width,
                                    gboolean expand_for_titles,
                                    gboolean outline);
void meta_ui_tab_popup_free(MetaTabPopup *popup);
void meta_ui_tab_popup_set_showing(MetaTabPopup *popup, gboolean showing);
void meta_ui_tab_popup_forward(MetaTabPopup *popup);
void meta_ui_tab_popup_backward(MetaTabPopup *popup);
void meta_ui_tab_popup_down(MetaTabPopup *popup);
void meta_ui_tab_popup_up(MetaTabPopup *popup);
MetaTabEntryKey meta_ui_tab_popup_get_selected(MetaTabPopup *popup);
void meta_ui_tab_popup_select(MetaTabPopup *popup, MetaTabEntryKey key);
GtkWidget *meta_ui_tab_popup_get_widget(MetaTabPopup *popup);
void meta_ui_tab_popup_mouse_press(MetaTabPopup *popup, gint x, gint y);

#endif
