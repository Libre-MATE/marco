/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (C) 2008 Iain Holmes
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

#ifndef META_DISPLAY_H
#define META_DISPLAY_H

#include <X11/Xlib.h>
#include <glib.h>

#include "types.h"

#define meta_XFree(p)    \
  do {                   \
    if ((p)) XFree((p)); \
  } while (0)

Display *meta_display_get_xdisplay(MetaDisplay *display);
MetaCompositor *meta_display_get_compositor(MetaDisplay *display);
GSList *meta_display_get_screens(MetaDisplay *display);

gboolean meta_display_has_shape(MetaDisplay *display);

MetaScreen *meta_display_screen_for_root(MetaDisplay *display, Window xroot);
MetaWindow *meta_display_get_focus_window(MetaDisplay *display);

int meta_display_get_damage_event_base(MetaDisplay *display);
int meta_display_get_shape_event_base(MetaDisplay *display);

#endif
