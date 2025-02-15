/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Marco X error handling */

/*
 * Copyright (C) 2001 Havoc Pennington, error trapping inspired by GDK
 * code copyrighted by the GTK team.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "errors.h"

#include <errno.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "display-private.h"

void meta_error_trap_push(MetaDisplay *display) {
  gdk_x11_display_error_trap_push(
      gdk_x11_lookup_xdisplay(meta_display_get_xdisplay(display)));
}

void meta_error_trap_pop(MetaDisplay *display,
                         gboolean last_request_was_roundtrip) {
  gdk_x11_display_error_trap_pop_ignored(
      gdk_x11_lookup_xdisplay(meta_display_get_xdisplay(display)));
}

int meta_error_trap_pop_with_return(MetaDisplay *display,
                                    gboolean last_request_was_roundtrip) {
  return gdk_x11_display_error_trap_pop(
      gdk_x11_lookup_xdisplay(meta_display_get_xdisplay(display)));
}
