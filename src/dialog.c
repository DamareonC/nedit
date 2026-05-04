#include "dialog.h"
#include "file_util.h"

static void post_file_unsaved_dialog(struct AppData* const app_data)
{
    switch (app_data->unsaved_type)
    {
        case CLOSE:
            exit(EXIT_SUCCESS);
        case NEW:
            new_file(app_data);
            break;
        case OPEN:
            file_open_dialog(app_data);
            break;
        default:
            break;
    }

    app_data->unsaved_type = NONE;
}

static void file_open_dialog_finish(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    GFile* const file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(object), async_result, nullptr);

    open_file(file, app_data);
    g_object_unref(file);
}

static void file_save_as_dialog_finish(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    GFile* const file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(object), async_result, nullptr);

    if (save_as_file(file, app_data) && app_data->unsaved_type != NONE)
        post_file_unsaved_dialog(app_data);

    g_object_unref(file);
}

static void file_unsaved_dialog_finished(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    const int result = gtk_alert_dialog_choose_finish(GTK_ALERT_DIALOG(object), async_result, nullptr);

    if (result == 0 || result == 1)
    {
        if (result == 0)
        {
            if (g_str_equal(app_data->file_name, "") || g_str_equal(app_data->file_path, ""))
            {
                file_save_as_dialog(app_data);
                return;
            }

            char* const full_path = g_strconcat(app_data->file_path, "/", app_data->file_name, NULL);
            GFile* const file = g_file_new_for_path(full_path);
            const bool file_saved = save_file(file, app_data);

            g_object_unref(file);
            g_free(full_path);

            if (!file_saved)
                return;

            g_free(app_data->file_content);
            app_data->file_content = get_buffer_text(app_data);
        }

        post_file_unsaved_dialog(app_data);
    }
}

void file_open_dialog(struct AppData* const app_data)
{
    GtkFileDialog* const file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(file_dialog, app_data->window, nullptr, file_open_dialog_finish, app_data);

    g_object_unref(file_dialog);
}

void file_save_as_dialog(struct AppData* const app_data)
{
    GtkFileDialog* const file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_save(file_dialog, app_data->window, nullptr, file_save_as_dialog_finish, app_data);

    g_object_unref(file_dialog);
}

void file_unsaved_dialog(struct AppData* const app_data)
{
    const char* message;

    switch (app_data->unsaved_type)
    {
        case NEW:
            message = "creating a new file";
            break;
        case OPEN:
            message = "opening another file";
            break;
        case CLOSE:
            message = "closing";
            break;
        default:
            message = "";
            break;
    }

    GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("There are unsaved changes in %s. Do you want to save changes before %s?", !g_str_equal(app_data->file_name, "") ? app_data->file_name : "<unnamed>", message);
    const char* const buttons[] = {"Yes", "No", "Cancel", nullptr};
    gtk_alert_dialog_set_buttons(alert_dialog, buttons);
    gtk_alert_dialog_set_default_button(alert_dialog, 0);
    gtk_alert_dialog_set_cancel_button(alert_dialog, 2);
    gtk_alert_dialog_choose(alert_dialog, app_data->window, nullptr, file_unsaved_dialog_finished, app_data);

    g_object_unref(alert_dialog);
}