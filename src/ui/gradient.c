/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/* Marco gradient rendering */

/*
 * Copyright (C) 2001 Havoc Pennington, 99% copied from wrlib in
 * WindowMaker, Copyright (C) 1997-2000 Dan Pascu and Alfredo Kojima
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
 * 02110-1301, USA.  */

/**
 * SECTION: gradient
 * @title: Gradients
 * @short_description: Metacity gradient rendering
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gradient.h"

#include <string.h>

/* Used as the destroy notification function for gdk_pixbuf_new() */
static void free_buffer(guchar *pixels, gpointer data) { g_free(pixels); }

static GdkPixbuf *blank_pixbuf(int width, int height) {
  guchar *buf;
  int rowstride;

  g_return_val_if_fail(width > 0, NULL);
  g_return_val_if_fail(height > 0, NULL);

  /* Always align rows to 32-bit boundaries */
  rowstride = 4 * ((4 * width + 4) / 4);

  buf = g_try_malloc(height * rowstride);
  if (!buf) return NULL;

  return gdk_pixbuf_new_from_data(buf, GDK_COLORSPACE_RGB, TRUE, 8, width,
                                  height, rowstride, free_buffer, NULL);
}

/*
 *----------------------------------------------------------------------
 * meta_gradient_create_horizontal--
 * 	Renders a horizontal linear gradient of the specified size in the
 * GdkPixbuf format with a border of the specified type.
 *
 * Returns:
 * 	A 24bit GdkPixbuf with the gradient (no alpha channel).
 *
 * Side effects:
 * 	None
 *----------------------------------------------------------------------
 */
static GdkPixbuf *meta_gradient_create_horizontal(int width, int height,
                                                  const GdkRGBA *from,
                                                  const GdkRGBA *to) {
  int i;
  long r, g, b, a, dr, dg, db, da;
  GdkPixbuf *pixbuf;
  unsigned char *ptr;
  unsigned char *pixels;
  int r0, g0, b0, a0;
  int rf, gf, bf, af;
  int rowstride;

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  ptr = pixels;
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  r0 = (guchar)(from->red * 0xff);
  g0 = (guchar)(from->green * 0xff);
  b0 = (guchar)(from->blue * 0xff);
  a0 = (guchar)(from->alpha * 0xff);
  rf = (guchar)(to->red * 0xff);
  gf = (guchar)(to->green * 0xff);
  bf = (guchar)(to->blue * 0xff);
  af = (guchar)(to->alpha * 0xff);

  r = r0 << 16;
  g = g0 << 16;
  b = b0 << 16;
  a = a0 << 16;

  dr = ((rf - r0) << 16) / (int)width;
  dg = ((gf - g0) << 16) / (int)width;
  db = ((bf - b0) << 16) / (int)width;
  da = ((af - a0) << 16) / (int)width;
  /* render the first line */
  for (i = 0; i < width; i++) {
    *(ptr++) = (unsigned char)(r >> 16);
    *(ptr++) = (unsigned char)(g >> 16);
    *(ptr++) = (unsigned char)(b >> 16);
    *(ptr++) = (unsigned char)(a >> 16);
    r += dr;
    g += dg;
    b += db;
    a += da;
  }

  /* copy the first line to the other lines */
  for (i = 1; i < height; i++) {
    memcpy(&(pixels[i * rowstride]), pixels, rowstride);
  }
  return pixbuf;
}

/*
 *----------------------------------------------------------------------
 * meta_gradient_create_vertical--
 *      Renders a vertical linear gradient of the specified size in the
 * GdkPixbuf format with a border of the specified type.
 *
 * Returns:
 *      A 24bit GdkPixbuf with the gradient (no alpha channel).
 *
 * Side effects:
 *      None
 *----------------------------------------------------------------------
 */
static GdkPixbuf *meta_gradient_create_vertical(int width, int height,
                                                const GdkRGBA *from,
                                                const GdkRGBA *to) {
  int i, j;
  long r, g, b, a, dr, dg, db, da;
  GdkPixbuf *pixbuf;
  unsigned char *ptr;
  int r0, g0, b0, a0;
  int rf, gf, bf, af;
  int rowstride;
  unsigned char *pixels;

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  r0 = (guchar)(from->red * 0xff);
  g0 = (guchar)(from->green * 0xff);
  b0 = (guchar)(from->blue * 0xff);
  a0 = (guchar)(from->alpha * 0xff);
  rf = (guchar)(to->red * 0xff);
  gf = (guchar)(to->green * 0xff);
  bf = (guchar)(to->blue * 0xff);
  af = (guchar)(to->alpha * 0xff);

  r = r0 << 16;
  g = g0 << 16;
  b = b0 << 16;
  a = a0 << 16;

  dr = ((rf - r0) << 16) / (int)height;
  dg = ((gf - g0) << 16) / (int)height;
  db = ((bf - b0) << 16) / (int)height;
  da = ((af - a0) << 16) / (int)height;

  for (i = 0; i < height; i++) {
    ptr = pixels + i * rowstride;

    ptr[0] = (unsigned char)(r >> 16);
    ptr[1] = (unsigned char)(g >> 16);
    ptr[2] = (unsigned char)(b >> 16);
    ptr[3] = (unsigned char)(a >> 16);

    for (j = 1; j <= width / 2; j *= 2) memcpy(&(ptr[j * 4]), ptr, j * 4);
    memcpy(&(ptr[j * 4]), ptr, (width - j) * 4);

    r += dr;
    g += dg;
    b += db;
    a += da;
  }
  return pixbuf;
}

/*
 *----------------------------------------------------------------------
 * meta_gradient_create_diagonal--
 *      Renders a diagonal linear gradient of the specified size in the
 * GdkPixbuf format with a border of the specified type.
 *
 * Returns:
 *      A 24bit GdkPixbuf with the gradient (no alpha channel).
 *
 * Side effects:
 *      None
 *----------------------------------------------------------------------
 */

static GdkPixbuf *meta_gradient_create_diagonal(int width, int height,
                                                const GdkRGBA *from,
                                                const GdkRGBA *to) {
  GdkPixbuf *pixbuf, *tmp;
  int j;
  float a, offset;
  unsigned char *ptr;
  unsigned char *pixels;
  int rowstride;

  if (width == 1)
    return meta_gradient_create_vertical(width, height, from, to);
  else if (height == 1)
    return meta_gradient_create_horizontal(width, height, from, to);

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  tmp = meta_gradient_create_horizontal(2 * width - 1, 1, from, to);
  if (!tmp) {
    g_object_unref(G_OBJECT(pixbuf));
    return NULL;
  }

  ptr = gdk_pixbuf_get_pixels(tmp);

  a = ((float)(width - 1)) / ((float)(height - 1));
  width = width * 4;

  /* copy the first line to the other lines with corresponding offset */
  for (j = 0, offset = 0.0; j < rowstride * height; j += rowstride) {
    memcpy(&(pixels[j]), &ptr[4 * (int)offset], width);
    offset += a;
  }

  g_object_unref(G_OBJECT(tmp));
  return pixbuf;
}

static GdkPixbuf *meta_gradient_create_multi_horizontal(int width, int height,
                                                        const GdkRGBA *colors,
                                                        int count) {
  int i, j, k;
  long r, g, b, a, dr, dg, db, da;
  GdkPixbuf *pixbuf;
  unsigned char *ptr;
  unsigned char *pixels;
  int width2;
  int rowstride;

  g_return_val_if_fail(count > 2, NULL);

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  ptr = pixels;

  if (count > width) count = width;

  if (count > 1)
    width2 = width / (count - 1);
  else
    width2 = width;

  k = 0;

  r = (long)(colors[0].red * 0xffffff);
  g = (long)(colors[0].green * 0xffffff);
  b = (long)(colors[0].blue * 0xffffff);
  a = (long)(colors[0].alpha * 0xffffff);

  /* render the first line */
  for (i = 1; i < count; i++) {
    dr = (int)((colors[i].red - colors[i - 1].red) * 0xffffff) / (int)width2;
    dg =
        (int)((colors[i].green - colors[i - 1].green) * 0xffffff) / (int)width2;
    db = (int)((colors[i].blue - colors[i - 1].blue) * 0xffffff) / (int)width2;
    da =
        (int)((colors[i].alpha - colors[i - 1].alpha) * 0xffffff) / (int)width2;
    for (j = 0; j < width2; j++) {
      *ptr++ = (unsigned char)(r >> 16);
      *ptr++ = (unsigned char)(g >> 16);
      *ptr++ = (unsigned char)(b >> 16);
      *ptr++ = (unsigned char)(a >> 16);
      r += dr;
      g += dg;
      b += db;
      a += da;
      k++;
    }
    r = (long)(colors[i].red * 0xffffff);
    g = (long)(colors[i].green * 0xffffff);
    b = (long)(colors[i].blue * 0xffffff);
    a = (long)(colors[i].alpha * 0xffffff);
  }
  for (j = k; j < width; j++) {
    *ptr++ = (unsigned char)(r >> 16);
    *ptr++ = (unsigned char)(g >> 16);
    *ptr++ = (unsigned char)(b >> 16);
    *ptr++ = (unsigned char)(a >> 16);
  }

  /* copy the first line to the other lines */
  for (i = 1; i < height; i++) {
    memcpy(&(pixels[i * rowstride]), pixels, rowstride);
  }
  return pixbuf;
}

static GdkPixbuf *meta_gradient_create_multi_vertical(int width, int height,
                                                      const GdkRGBA *colors,
                                                      int count) {
  int i, j, k;
  long r, g, b, a, dr, dg, db, da;
  GdkPixbuf *pixbuf;
  unsigned char *ptr, *tmp, *pixels;
  int height2;
  int x;
  int rowstride;

  g_return_val_if_fail(count > 2, NULL);

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  ptr = pixels;

  if (count > height) count = height;

  if (count > 1)
    height2 = height / (count - 1);
  else
    height2 = height;

  k = 0;

  r = (long)(colors[0].red * 0xffffff);
  g = (long)(colors[0].green * 0xffffff);
  b = (long)(colors[0].blue * 0xffffff);
  a = (long)(colors[0].alpha * 0xffffff);

  for (i = 1; i < count; i++) {
    dr = (int)((colors[i].red - colors[i - 1].red) * 0xffffff) / (int)height2;
    dg = (int)((colors[i].green - colors[i - 1].green) * 0xffffff) /
         (int)height2;
    db = (int)((colors[i].blue - colors[i - 1].blue) * 0xffffff) / (int)height2;
    da = (int)((colors[i].alpha - colors[i - 1].alpha) * 0xffffff) /
         (int)height2;

    for (j = 0; j < height2; j++) {
      ptr[0] = (unsigned char)(r >> 16);
      ptr[1] = (unsigned char)(g >> 16);
      ptr[2] = (unsigned char)(b >> 16);
      ptr[3] = (unsigned char)(a >> 16);

      for (x = 1; x <= width / 2; x *= 2) memcpy(&(ptr[x * 4]), ptr, x * 4);
      memcpy(&(ptr[x * 4]), ptr, (width - x) * 4);

      ptr += rowstride;

      r += dr;
      g += dg;
      b += db;
      a += da;
      k++;
    }
    r = (long)(colors[i].red * 0xffffff);
    g = (long)(colors[i].green * 0xffffff);
    b = (long)(colors[i].blue * 0xffffff);
    a = (long)(colors[i].alpha * 0xffffff);
  }

  if (k < height) {
    tmp = ptr;

    ptr[0] = (unsigned char)(r >> 16);
    ptr[1] = (unsigned char)(g >> 16);
    ptr[2] = (unsigned char)(b >> 16);
    ptr[3] = (unsigned char)(a >> 16);

    for (x = 1; x <= width / 2; x *= 2) memcpy(&(ptr[x * 4]), ptr, x * 4);
    memcpy(&(ptr[x * 4]), ptr, (width - x) * 4);

    ptr += rowstride;

    for (j = k + 1; j < height; j++) {
      memcpy(ptr, tmp, rowstride);
      ptr += rowstride;
    }
  }

  return pixbuf;
}

static GdkPixbuf *meta_gradient_create_multi_diagonal(int width, int height,
                                                      const GdkRGBA *colors,
                                                      int count) {
  GdkPixbuf *pixbuf, *tmp;
  float a, offset;
  int j;
  unsigned char *ptr;
  unsigned char *pixels;
  int rowstride;

  g_return_val_if_fail(count > 2, NULL);

  if (width == 1)
    return meta_gradient_create_multi_vertical(width, height, colors, count);
  else if (height == 1)
    return meta_gradient_create_multi_horizontal(width, height, colors, count);

  pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  if (count > width) count = width;
  if (count > height) count = height;

  if (count > 2)
    tmp =
        meta_gradient_create_multi_horizontal(2 * width - 1, 1, colors, count);
  else
    /* wrlib multiplies these colors by 256 before passing them in, but
     * I think it's a bug in wrlib, so changed here. I could be wrong
     * though, if we notice two-color multi diagonals not working.
     */
    tmp = meta_gradient_create_horizontal(2 * width - 1, 1, &colors[0],
                                          &colors[1]);

  if (!tmp) {
    g_object_unref(G_OBJECT(pixbuf));
    return NULL;
  }
  ptr = gdk_pixbuf_get_pixels(tmp);

  a = ((float)(width - 1)) / ((float)(height - 1));
  width = width * 3;

  /* copy the first line to the other lines with corresponding offset */
  for (j = 0, offset = 0; j < rowstride * height; j += rowstride) {
    memcpy(&(pixels[j]), &ptr[3 * (int)offset], width);
    offset += a;
  }

  g_object_unref(G_OBJECT(tmp));
  return pixbuf;
}

static void simple_multiply_alpha(GdkPixbuf *pixbuf, guchar alpha) {
  guchar *pixels;
  int rowstride;
  int height;
  int row;

  g_return_if_fail(GDK_IS_PIXBUF(pixbuf));

  if (alpha == 255) return;

  g_assert(gdk_pixbuf_get_has_alpha(pixbuf));

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);
  height = gdk_pixbuf_get_height(pixbuf);

  row = 0;
  while (row < height) {
    guchar *p;
    guchar *end;

    p = pixels + row * rowstride;
    end = p + rowstride;

    while (p != end) {
      p += 3; /* skip RGB */

      /* multiply the two alpha channels. not sure this is right.
       * but some end cases are that if the pixbuf contains 255,
       * then it should be modified to contain "alpha"; if the
       * pixbuf contains 0, it should remain 0.
       */
      /* ((*p / 255.0) * (alpha / 255.0)) * 255; */
      *p = (guchar)(((int)*p * (int)alpha) / (int)255);

      ++p; /* skip A */
    }

    ++row;
  }
}

static void meta_gradient_add_alpha_horizontal(GdkPixbuf *pixbuf,
                                               const unsigned char *alphas,
                                               int n_alphas) {
  int i, j;
  long a, da;
  unsigned char *p;
  unsigned char *pixels;
  int width2;
  int rowstride;
  int width, height;
  unsigned char *gradient;
  unsigned char *gradient_p;
  unsigned char *gradient_end;

  g_return_if_fail(n_alphas > 0);

  if (n_alphas == 1) {
    /* Optimize this */
    simple_multiply_alpha(pixbuf, alphas[0]);
    return;
  }

  width = gdk_pixbuf_get_width(pixbuf);
  height = gdk_pixbuf_get_height(pixbuf);

  gradient = g_new(unsigned char, width);
  gradient_end = gradient + width;

  if (n_alphas > width) n_alphas = width;

  if (n_alphas > 1)
    width2 = width / (n_alphas - 1);
  else
    width2 = width;

  a = alphas[0] << 8;
  gradient_p = gradient;

  /* render the gradient into an array */
  for (i = 1; i < n_alphas; i++) {
    da = (((int)(alphas[i] - (int)alphas[i - 1])) << 8) / (int)width2;

    for (j = 0; j < width2; j++) {
      *gradient_p++ = (a >> 8);

      a += da;
    }

    a = alphas[i] << 8;
  }

  /* get leftover pixels */
  while (gradient_p != gradient_end) {
    *gradient_p++ = a >> 8;
  }

  /* Now for each line of the pixbuf, fill in with the gradient */
  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

  p = pixels;
  i = 0;
  while (i < height) {
    unsigned char *row_end = p + rowstride;
    gradient_p = gradient;

    p += 3;
    while (gradient_p != gradient_end) {
      /* multiply the two alpha channels. not sure this is right.
       * but some end cases are that if the pixbuf contains 255,
       * then it should be modified to contain "alpha"; if the
       * pixbuf contains 0, it should remain 0.
       */
      /* ((*p / 255.0) * (alpha / 255.0)) * 255; */
      *p = (guchar)(((int)*p * (int)*gradient_p) / (int)255);

      p += 4;
      ++gradient_p;
    }

    p = row_end;
    ++i;
  }

  g_free(gradient);
}

/**
 * meta_gradient_create_simple:
 * @width: Width in pixels
 * @height: Height in pixels
 * @from: Starting color
 * @to: Ending color
 * @style: Gradient style
 *
 * Returns: (transfer full): A new linear gradient
 */
GdkPixbuf *meta_gradient_create_simple(int width, int height,
                                       const GdkRGBA *from, const GdkRGBA *to,
                                       MetaGradientType style) {
  switch (style) {
    case META_GRADIENT_HORIZONTAL:
      return meta_gradient_create_horizontal(width, height, from, to);
    case META_GRADIENT_VERTICAL:
      return meta_gradient_create_vertical(width, height, from, to);

    case META_GRADIENT_DIAGONAL:
      return meta_gradient_create_diagonal(width, height, from, to);
    case META_GRADIENT_LAST:
      break;

    default:
      break;
  }
  g_assert_not_reached();
  return NULL;
}

/**
 * meta_gradient_create_multi:
 * @width: Width in pixels
 * @height: Height in pixels
 * @colors: (array length=n_colors): Array of colors
 * @n_colors: Number of colors
 * @style: Gradient style
 *
 * Returns: (transfer full): A new multi-step linear gradient
 */
GdkPixbuf *meta_gradient_create_multi(int width, int height,
                                      const GdkRGBA *colors, int n_colors,
                                      MetaGradientType style) {
  if (n_colors > 2) {
    switch (style) {
      case META_GRADIENT_HORIZONTAL:
        return meta_gradient_create_multi_horizontal(width, height, colors,
                                                     n_colors);
      case META_GRADIENT_VERTICAL:
        return meta_gradient_create_multi_vertical(width, height, colors,
                                                   n_colors);
      case META_GRADIENT_DIAGONAL:
        return meta_gradient_create_multi_diagonal(width, height, colors,
                                                   n_colors);
      case META_GRADIENT_LAST:
        g_assert_not_reached();
        break;
      default:
        g_assert_not_reached();
        break;
    }
  } else if (n_colors > 1) {
    return meta_gradient_create_simple(width, height, &colors[0], &colors[1],
                                       style);
  } else if (n_colors > 0) {
    return meta_gradient_create_simple(width, height, &colors[0], &colors[0],
                                       style);
  }
  g_assert_not_reached();
  return NULL;
}

/**
 * meta_gradient_create_interwoven: (skip)
 * @width: Width in pixels
 * @height: Height in pixels
 * @colors1: Array of colors
 * @thickness1: Thickness
 * @colors2: Array of colors
 * @thickness2: Thickness
 *
 * Interwoven essentially means we have two vertical gradients,
 * cut into horizontal strips of the given thickness, and then the strips
 * are alternated. I'm not sure what it's good for, just copied since
 * WindowMaker had it.
 */
GdkPixbuf *meta_gradient_create_interwoven(int width, int height,
                                           const GdkRGBA colors1[2],
                                           int thickness1,
                                           const GdkRGBA colors2[2],
                                           int thickness2) {
  int i, j, k, l, ll;
  long r1, g1, b1, a1, dr1, dg1, db1, da1;
  long r2, g2, b2, a2, dr2, dg2, db2, da2;
  GdkPixbuf *pixbuf;
  unsigned char *ptr;
  unsigned char *pixels;
  int rowstride;
  double height_scaling;

  pixbuf = blank_pixbuf(width, height);
  if (pixbuf == NULL) return NULL;

  pixels = gdk_pixbuf_get_pixels(pixbuf);
  rowstride = gdk_pixbuf_get_rowstride(pixbuf);

#define RGBA_SCALING 16777215.0 /* 0xffffff */
  r1 = (long)(colors1[0].red * RGBA_SCALING);
  g1 = (long)(colors1[0].green * RGBA_SCALING);
  b1 = (long)(colors1[0].blue * RGBA_SCALING);
  a1 = (long)(colors1[0].alpha * RGBA_SCALING);

  r2 = (long)(colors2[0].red * RGBA_SCALING);
  g2 = (long)(colors2[0].green * RGBA_SCALING);
  b2 = (long)(colors2[0].blue * RGBA_SCALING);
  a2 = (long)(colors2[0].alpha * RGBA_SCALING);

  height_scaling = RGBA_SCALING / (gdouble)height;

  dr1 = (long)(colors1[1].red - colors1[0].red * height_scaling);
  dg1 = (long)(colors1[1].green - colors1[0].green * height_scaling);
  db1 = (long)(colors1[1].blue - colors1[0].blue * height_scaling);
  da1 = (long)(colors1[1].alpha - colors1[0].alpha * height_scaling);

  dr2 = (long)(colors2[1].red - colors2[0].red * height_scaling);
  dg2 = (long)(colors2[1].green - colors2[0].green * height_scaling);
  db2 = (long)(colors2[1].blue - colors2[0].blue * height_scaling);
  da2 = (long)(colors2[1].alpha - colors2[0].alpha * height_scaling);
#undef RGBA_SCALING

  for (i = 0, k = 0, l = 0, ll = thickness1; i < height; i++) {
    ptr = pixels + i * rowstride;

    if (k == 0) {
      ptr[0] = (unsigned char)(r1 >> 16);
      ptr[1] = (unsigned char)(g1 >> 16);
      ptr[2] = (unsigned char)(b1 >> 16);
      ptr[3] = (unsigned char)(a1 >> 16);
    } else {
      ptr[0] = (unsigned char)(r2 >> 16);
      ptr[1] = (unsigned char)(g2 >> 16);
      ptr[2] = (unsigned char)(b2 >> 16);
      ptr[3] = (unsigned char)(a2 >> 16);
    }

    for (j = 1; j <= width / 2; j *= 2) memcpy(&(ptr[j * 4]), ptr, j * 4);
    memcpy(&(ptr[j * 4]), ptr, (width - j) * 4);

    if (++l == ll) {
      if (k == 0) {
        k = 1;
        ll = thickness2;
      } else {
        k = 0;
        ll = thickness1;
      }
      l = 0;
    }
    r1 += dr1;
    g1 += dg1;
    b1 += db1;
    a1 += da1;

    r2 += dr2;
    g2 += dg2;
    b2 += db2;
    a2 += da2;
  }

  return pixbuf;
}

void meta_gradient_add_alpha(GdkPixbuf *pixbuf, const guchar *alphas,
                             int n_alphas, MetaGradientType type) {
  g_return_if_fail(GDK_IS_PIXBUF(pixbuf));
  g_return_if_fail(gdk_pixbuf_get_has_alpha(pixbuf));
  g_return_if_fail(n_alphas > 0);

  switch (type) {
    case META_GRADIENT_HORIZONTAL:
      meta_gradient_add_alpha_horizontal(pixbuf, alphas, n_alphas);
      break;

    case META_GRADIENT_VERTICAL:
      g_printerr(
          "marco: vertical alpha channel gradient not implemented yet\n");
      break;

    case META_GRADIENT_DIAGONAL:
      g_printerr(
          "marco: diagonal alpha channel gradient not implemented yet\n");
      break;

    case META_GRADIENT_LAST:
      g_assert_not_reached();
      break;

    default:
      g_assert_not_reached();
      break;
  }
}
