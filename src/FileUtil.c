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

static void file_save(GSimpleAction* const action, GVariant* const param, const gpointer data)
{
    struct AppData* const app_data = data;

    if (!is_unsaved(app_data))
    {
        return;
    }

    if (g_str_equal(app_data->file_name, "") || g_str_equal(app_data->file_path, ""))
    {
        file_save_as_dialog(app_data);
    }
    else
    {
        char* const full_path = g_strconcat(app_data->file_path, "/", app_data->file_name, NULL);
        GFile* const file = g_file_new_for_path(full_path);

        if (save_file(file, app_data))
        {
            g_free(app_data->file_content);
            app_data->file_content = get_buffer_text(app_data);
        }

        g_free(full_path);
    }
}

static void file_save_as(GSimpleAction* const action, GVariant* const param, const gpointer data)
{
    struct AppData* const app_data = data;
    file_save_as_dialog(app_data);
}

void init_file_menu(GtkApplication* const app, struct AppData* const app_data)
{
    const GActionEntry action_entries[] = {
        { "file-new", file_new, NULL, NULL, NULL },
        { "file-open", file_open, NULL, NULL, NULL },
        { "file-save", file_save, NULL, NULL, NULL },
        { "file-save-as", file_save_as, NULL, NULL, NULL }
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

bool save_file(GFile* const file, const struct AppData* const app_data)
{
    char* const buffered_file_content = get_buffer_text(app_data);
    const glong content_length = g_utf8_strlen(buffered_file_content, -1);
    bool result;
    
    if (g_file_replace_contents(file, buffered_file_content, content_length, NULL, false, G_FILE_CREATE_NONE, NULL, NULL, NULL))
    {
        result = true;
    }
    else
    {
        GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("Could not save the file.");
        gtk_alert_dialog_show(alert_dialog, app_data->window);
        
        g_object_unref(alert_dialog);
        result = false;
    }

    g_free(buffered_file_content);
    g_object_unref(file);
    return result;
}

bool save_as_file(GFile* const file, struct AppData* const app_data)
{
    if (file)
    {
        GFile* const parent = g_file_get_parent(file);
        set_app_data(app_data, g_file_get_basename(file), g_file_get_path(parent), get_buffer_text(app_data));

        g_object_unref(parent);
        return save_file(file, app_data);
    }
    else
    {
        return false;
    }
}

bool is_unsaved(const struct AppData* const app_data)
{
    char* const buffered_file_content = get_buffer_text(app_data);
    const bool result = !g_str_equal(app_data->file_content, buffered_file_content);

    g_free(buffered_file_content);
    return result;
}

char* get_buffer_text(const struct AppData* const app_data)
{
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(app_data->text_buffer, &start);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(app_data->text_buffer, &end);

    return gtk_text_buffer_get_text(app_data->text_buffer, &start, &end, false);
}