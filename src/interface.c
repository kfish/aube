#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

void
close_window_cb(GtkAccelGroup * accel_group,
		GObject * acceleratable,
		guint keyval, GdkModifierType modifier)
{
	if (GTK_IS_WINDOW(acceleratable))
		g_signal_emit_by_name(G_OBJECT(acceleratable), "destroy");
}


/*
 * close the window when Ctrl and w are pressed.
 *      accel key combo is static. perhaps there is a better
 *      way to do this?
 */
void attach_window_close_accel(GtkWindow * window)
{
	GClosure *gclosure;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new();
	gclosure = g_cclosure_new((GCallback) close_window_cb, NULL, NULL);
	gtk_accel_group_connect(accel_group, GDK_w, GDK_CONTROL_MASK, 0,	/* non of the GtkAccelFlags seem suitable? */
				gclosure);
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);
}
