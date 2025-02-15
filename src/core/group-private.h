/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Marco window group private header */

/*
 * Copyright (C) 2002 Red Hat Inc.
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

#ifndef META_GROUP_PRIVATE_H
#define META_GROUP_PRIVATE_H

#include "group.h"

struct _MetaGroup {
  int refcount;
  MetaDisplay *display;
  GSList *windows;
  Window group_leader;
  char *startup_id;
  char *wm_client_machine;
};

#endif
