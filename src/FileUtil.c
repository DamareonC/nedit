#include "Dialog.h"
#include "FileUtil.h"

static void file_new(GSimpleAction* const action, GVariant* const param, const gpointer data)
{
    struct AppData* const app_data = data;
    
    if (is_unsaved(app_data))
    {
        app_data->unsaved_type = NEW;
        file_unsaved_dialog(app_data);
    }
    else
    {
        new_file(app_data);
    }
}

static void file_open(GSimpleAction* const action, GVariant* const param, const gpointer data)
{
    struct AppData* const app_data = data;

    if (is_unsaved(app_data))
    {
        app_data->unsaved_type = OPEN;
        file_unsaved_dialog(app_data);
    }
    else
    {
        file_open_dialog(app_data);
    }
}

void init_file_menu(GtkApplication* const app, struct AppData* const app_data)
{
    const GActionEntry action_entries[] = {
        { "file-new", file_new, NULL, NULL, NULL },
        { "file-open", file_open, NULL, NULL, NULL }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), action_entries, G_N_ELEMENTS(action_entries), app_data);
}

void new_file(struct AppData* const app_data)
{
    set_app_data(app_data, g_malloc0(sizeof(char)), g_malloc0(sizeof(char)), g_malloc0(sizeof(char)));
    gtk_text_buffer_set_text(app_data->text_buffer, "", 0);
}

void open_file(GFile* const file, struct AppData* const app_data)
{
    char* content;
    gsize length;

    if (file)
    {
        if (g_file_load_contents(file, NULL, &content, &length, NULL, NULL))
        {
            GFile* const parent = g_file_get_parent(file);

            set_app_data(app_data, g_file_get_basename(file), g_file_get_path(parent), content);
            gtk_text_buffer_set_text(app_data->text_buffer, content, length);

            g_object_unref(parent);
        }
        else
        {
            GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("Could not open the file.");
            gtk_alert_dialog_show(alert_dialog, app_data->window);
            
            g_object_unref(alert_dialog);
        }
    
        g_object_unref(file);
    }
}

bool is_unsaved(struct AppData* const app_data)
{
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(app_data->text_buffer, &start);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(app_data->text_buffer, &end);

    char* const buffered_file_content = gtk_text_buffer_get_text(app_data->text_buffer, &start, &end, false);
    bool result = !g_str_equal(app_data->file_content, buffered_file_content);

    g_free(buffered_file_content);
    return result;
}