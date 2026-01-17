#include "Dialog.h"
#include "FileUtil.h"

static void file_open_dialog_finish(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    
    GFile* const file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(object), async_result, NULL);
    open_file(file, app_data);
}

static void file_unsaved_dialog_finished(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    int result = gtk_alert_dialog_choose_finish(GTK_ALERT_DIALOG(object), async_result, NULL);
    
    if (result == 0 || result == 1)
    {
        if (result == 0)
        {
            const bool file_saved = save_file(app_data);

            if (!file_saved)
            {
                return;
            }
        }

        switch (app_data->unsaved_type)
        {
            case CLOSE:
                exit(EXIT_SUCCESS);
                break;
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
}

void file_open_dialog(struct AppData* const app_data)
{
    GtkFileDialog* const file_dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(file_dialog, app_data->window, NULL, file_open_dialog_finish, app_data);

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
            break;
    }

    GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("There are unsaved changes in %s. Do you want to save changes before %s?", !g_str_equal(app_data->file_name, "") ? app_data->file_name : "<unnamed>", message);
    const char* const buttons[] = { "Yes", "No", "Cancel", NULL };
    gtk_alert_dialog_set_buttons(alert_dialog, buttons);
    gtk_alert_dialog_set_default_button(alert_dialog, 0);
    gtk_alert_dialog_set_cancel_button(alert_dialog, 2);
    gtk_alert_dialog_choose(alert_dialog, app_data->window, NULL, file_unsaved_dialog_finished, app_data);

    g_object_unref(alert_dialog);
}