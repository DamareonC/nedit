#include <gtk/gtk.h>

static void init(GtkApplication* const app, const gpointer data)
{
    GtkBuilder* const builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "res/main_window.ui", NULL);

    GtkWindow* const main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_application(main_window, app);
    gtk_window_present(main_window);

    g_object_unref(builder);
}