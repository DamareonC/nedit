#pragma once

#include "FileUtil.h"

static void init_app(GtkApplication* const app, const gpointer data)
{
    GtkBuilder* const builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "res/main_window.ui", NULL);

    GtkWindow* const main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_application(main_window, app);
    gtk_window_present(main_window);

    static struct AppData app_data = {
        .file_name = "",
        .file_path = "",
        .file_content = ""
    };
    app_data.window = main_window;
    app_data.text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "file_text_view")));

    init_file_menu(app, &app_data);

    g_object_unref(builder);
}