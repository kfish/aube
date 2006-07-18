/*
   klav.h - GTK+ "Klav" Widget
   * Copyright (C) 1998 Simon Kågedal
   *
   * This program is free software; you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation; either version 2 of the License, or
   * (at your option) any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License in the file COPYING for more details.
   *
   * created 1998-04-19
 */

#ifndef __KLAV_H__
#define __KLAV_H__

#include <gdk/gdk.h>
#include <gtk/gtkdrawingarea.h>

#ifdef __cplusplus
extern "C" {
#endif				/* __cplusplus */

#define KLAV(obj)	GTK_CHECK_CAST (obj, klav_get_type (), Klav)
#define KLAV_CLASS(klass) GTK_CHECK_CLASS_CAST (klass, klav_get_type, KlavClass)
#define IS_KLAV(obj)	GTK_CHECK_TYPE (obj, klav_get_type ())


  typedef struct _Klav Klav;
  typedef struct _KlavClass KlavClass;

  typedef enum {
    KLAV_TYPE_SEQUENCER,
    KLAV_TYPE_NORMAL
  } KlavType;

  typedef enum {
    KLAV_DIR_VERTICAL,
    KLAV_DIR_HORIZONTAL
  } KlavDir;

  struct _KlavKeyInfo {
    gboolean is_black;		/*
				   is it a black key 
				 */

    /*
       these two contains the x value of the right side of the key 
       * (it's actually y if klav->dir == KLAV_DIR_VERTICAL) 
     */

    gint upper_right_x;
    gint lower_right_x;		/*
				   not valid if is_black==TRUE 
				 */
  };

  typedef struct _KlavKeyInfo KlavKeyInfo;

  struct _Klav {
    GtkDrawingArea drawingarea;

    KlavType type;
    KlavDir dir;
    gint key_start;		/*
				   key number start. like MIDI, middle C = 60 
				 */
    gint key_end;		/*
				   key number end. 
				 */
    gfloat black_key_height;	/*
				   percentage of the whole height 
				 */
    gfloat black_key_width;	/*
				   when type==KLAV_TYPE_NORMAL 
				 */

    gboolean is_pressed;	/*
				   is a key pressed? 
				 */
    gint key_pressed;		/*
				   which key? 
				 */

    /*
       stuff that is calculated 
     */

    gfloat keywidth;		/*
				   X if dir==KLAV_DIR_HORIZONTAL 
				 */

    KlavKeyInfo *key_info;
    gint key_info_size;
  };

  struct _KlavClass {
    GtkDrawingAreaClass parent_class;

    void (*klavkey_press) (Klav * klav, gint key);
    void (*klavkey_release) (Klav * klav);
  };

  GType klav_get_type(void);
  GtkWidget *klav_new(void);

  void klav_set_range(Klav * klav, gint start, gint end);
  void klav_set_klav_type(Klav * klav, KlavType type);
  void klav_set_klav_dir(Klav * klav, KlavDir dir);

  void klav_press(Klav * klav, gint key);
  void klav_release(Klav * klav, gint key);

#ifdef __cplusplus
}

#endif				/*
				   __cplusplus 
				 */
#endif				/*
				   __KLAVIATUR_H__ 
				 */
