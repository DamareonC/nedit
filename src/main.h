#pragma once

#include "dialog.h"
#include "file_util.h"

static constexpr char main_window_ui[] = {
    #embed "main_window.ui"
};

static gboolean close_request(const GtkWindow* const, const gpointer data)
{
    struct AppData* const app_data = data;

    if (is_unsaved(app_data))
    {
        app_data->unsaved_type = CLOSE;
        file_unsaved_dialog(app_data);

        return true;
    }

    return false;
}

static void changed(const GtkTextBuffer* const, const gpointer data)
{
    struct AppData* const app_data = data;
    set_window_title(g_strdup_printf("NEdit - %s%s", g_str_equal(app_data->file_name, "") ? "<unnamed>" : app_data->file_name, is_unsaved(app_data) ? "*" : ""), app_data);
}

static void start(GtkApplication* const app, const gpointer data)
{
    GtkBuilder* const builder = gtk_builder_new_from_string(main_window_ui, -1);
    GObject* const main_window = gtk_builder_get_object(builder, "main_window");
    gtk_window_set_application(GTK_WINDOW(main_window), app);
    gtk_window_present(GTK_WINDOW(main_window));

    struct AppData* const app_data = data;
    app_data->window = GTK_WINDOW(main_window);
    app_data->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "file_text_view")));
    g_signal_connect(app_data->window, "close-request", G_CALLBACK(close_request), app_data);
    g_signal_connect(app_data->text_buffer, "changed", G_CALLBACK(changed), app_data);
    set_window_title(g_strdup_printf("NEdit - %s", g_str_equal(app_data->file_name, "") ? "<unnamed>" : app_data->file_name), app_data);

    init_file_menu(app, app_data);

    g_object_unref(builder);
}

static void start_with_files(GtkApplication* const app, const gpointer files, const gint n_files, const gchar* const, const gpointer data)
{
    start(app, data);

    struct AppData* const app_data = data;

    if (n_files > 0)
    {
        GFile** const g_files = files;
        open_file(g_files[0], app_data);
    }
}