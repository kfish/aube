#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <gtk/gtk.h>

/*
 * close the window when Ctrl and w are pressed.
 *      accel key combo is static. perhaps there is a better
 *      way to do this?
 */
void attach_window_close_accel(GtkWindow * window);

#endif /* __INTERFACE_H__ */
