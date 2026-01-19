#pragma once

#include "Dialog.h"
#include "FileUtil.h"

static gboolean close_request(const GtkWindow* const window, const gpointer data)
{
    struct AppData* const app_data = data;

    if (is_unsaved(app_data))
    {
        app_data->unsaved_type = CLOSE;
        file_unsaved_dialog(app_data);

        return true;
    }
    else
    {
        return false;
    }
}

static void changed(const GtkTextBuffer* const text_buffer, const gpointer data)
{
    struct AppData* const app_data = data;
    gtk_window_set_title(app_data->window, g_strdup_printf("NEdit - %s%s", g_str_equal(app_data->file_name, "") ? "<unnamed>" : app_data->file_name, is_unsaved(app_data) ? "*" : ""));
}

static void start(GtkApplication* const app, const gpointer data)
{
    GtkBuilder* const builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "res/main_window.ui", NULL);

    GtkWindow* const main_window = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_application(main_window, app);
    gtk_window_present(main_window);

    struct AppData* const app_data = data;
    app_data->window = main_window;
    app_data->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "file_text_view")));
    g_signal_connect(app_data->window, "close-request", G_CALLBACK(close_request), app_data);
    g_signal_connect(app_data->text_buffer, "changed", G_CALLBACK(changed), app_data);
    gtk_window_set_title(app_data->window, g_strdup_printf("NEdit - %s", g_str_equal(app_data->file_name, "") ? "<unnamed>" : app_data->file_name));

    init_file_menu(app, app_data);

    g_object_unref(builder);
}

static void start_with_files(GtkApplication* const app, const gpointer files, const gint n_files, const gchar* const hint, const gpointer data)
{
    start(app, data);

    struct AppData* const app_data = data;

    if (n_files > 0)
    {
        GFile** const g_files = files;
        open_file(g_files[0], app_data);
    }
}