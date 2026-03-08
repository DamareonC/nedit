#include "dialog.h"
#include "file_util.h"

static void file_new(GSimpleAction* const, GVariant* const, const gpointer data)
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

static void file_open(GSimpleAction* const, GVariant* const, const gpointer data)
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

static void file_reload(GSimpleAction* const, GVariant* const, const gpointer data)
{
    struct AppData* const app_data = data;

    if (!g_str_equal(app_data->file_name, "") && !g_str_equal(app_data->file_path, ""))
    {
        char* const full_path = g_strconcat(app_data->file_path, "/", app_data->file_name, nullptr);
        GFile* const file = g_file_new_for_path(full_path);

        open_file(file, app_data);

        g_object_unref(file);
        g_free(full_path);
    }
}

static void file_save(GSimpleAction* const, GVariant* const, const gpointer data)
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
        char* const full_path = g_strconcat(app_data->file_path, "/", app_data->file_name, nullptr);
        GFile* const file = g_file_new_for_path(full_path);

        if (save_file(file, app_data))
        {
            g_free(app_data->file_content);
            app_data->file_content = get_buffer_text(app_data);
        }

        g_object_unref(file);
        g_free(full_path);
    }
}

static void file_save_as(GSimpleAction* const, GVariant* const, const gpointer data)
{
    struct AppData* const app_data = data;
    file_save_as_dialog(app_data);
}

void init_file_menu(GtkApplication* const app, struct AppData* const app_data)
{
    const GActionEntry action_entries[] = {
        { "file-new", file_new, nullptr, nullptr, nullptr },
        { "file-open", file_open, nullptr, nullptr, nullptr },
        { "file-reload", file_reload, nullptr, nullptr, nullptr },
        { "file-save", file_save, nullptr, nullptr, nullptr },
        { "file-save-as", file_save_as, nullptr, nullptr, nullptr }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), action_entries, G_N_ELEMENTS(action_entries), app_data);
}

void new_file(struct AppData* const app_data)
{
    gtk_window_set_title(app_data->window, "NEdit - <unnamed>");
    gtk_text_buffer_set_text(app_data->text_buffer, "", 0);
    set_app_data(g_malloc0(sizeof(char)), g_malloc0(sizeof(char)), g_malloc0(sizeof(char)), app_data);
}

void open_file(GFile* const file, struct AppData* const app_data)
{
    char* content;
    gsize length;

    if (file)
    {
        if (g_file_load_contents(file, nullptr, &content, &length, nullptr, nullptr))
        {
            GFile* const parent = g_file_get_parent(file);

            gtk_window_set_title(app_data->window, g_strdup_printf("NEdit - %s", app_data->file_name));
            gtk_text_buffer_set_text(app_data->text_buffer, content, (int)length);
            set_app_data(g_file_get_basename(file), g_file_get_path(parent), content, app_data);

            g_object_unref(parent);
        }
        else
        {
            GtkAlertDialog* const alert_dialog = gtk_alert_dialog_new("Could not open the file.");
            gtk_alert_dialog_show(alert_dialog, app_data->window);
            
            g_object_unref(alert_dialog);
        }
    }
}

bool save_file(GFile* const file, const struct AppData* const app_data)
{
    char* const buffered_file_content = get_buffer_text(app_data);
    const glong content_length = g_utf8_strlen(buffered_file_content, -1);
    bool result;
    
    if (g_file_replace_contents(file, buffered_file_content, content_length, nullptr, false, G_FILE_CREATE_NONE, nullptr, nullptr, nullptr))
    {
        gtk_window_set_title(app_data->window, g_strdup_printf("NEdit - %s", app_data->file_name));
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
    return result;
}

bool save_as_file(GFile* const file, struct AppData* const app_data)
{
    if (file)
    {
        GFile* const parent = g_file_get_parent(file);
        set_app_data(g_file_get_basename(file), g_file_get_path(parent), get_buffer_text(app_data), app_data);

        g_object_unref(parent);
        return save_file(file, app_data);
    }

    return false;
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