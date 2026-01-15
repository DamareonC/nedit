#include "Dialog.h"
#include "FileUtil.h"

enum Option
{
    YES, NO, CANCEL
};

static void file_new_dialog_finish(GObject* const object, GAsyncResult* const async_result, const gpointer data)
{
    struct AppData* const app_data = data;
    int result = gtk_alert_dialog_choose_finish(GTK_ALERT_DIALOG(object), async_result, NULL);
    
    if (result == YES || result == NO)
    {
        if (result == YES)
        {
            //Save function here
        }

        new_file(app_data);
    }
}

void file_new_dialog(struct AppData* const app_data)
{
    GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("There are unsaved changes in %s. Do you want to save changes before creating a new file?", !g_str_equal(app_data->file_name, "") ? app_data->file_name : "<unnamed>");
    gtk_alert_dialog_set_modal(alert_dialog, true);

    const char* const buttons[] = { "Yes", "No", "Cancel", NULL };
    gtk_alert_dialog_set_buttons(alert_dialog, buttons);
    gtk_alert_dialog_set_default_button(alert_dialog, YES);
    gtk_alert_dialog_set_cancel_button(alert_dialog, CANCEL);
    gtk_alert_dialog_choose(alert_dialog, app_data->window, NULL, file_new_dialog_finish, app_data);

    g_object_unref(alert_dialog);
}