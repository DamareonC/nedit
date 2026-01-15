#include "Dialog.h"
#include "FileUtil.h"

static void file_new(GSimpleAction* const action, GVariant* const param, const gpointer data)
{
    struct AppData* const app_data = data;
    
    GtkTextIter start;
    gtk_text_buffer_get_start_iter(app_data->text_buffer, &start);

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(app_data->text_buffer, &end);

    char* const buffered_file_content = gtk_text_buffer_get_text(app_data->text_buffer, &start, &end, false);

    if (!g_str_equal(app_data->file_content, buffered_file_content))
    {
        file_new_dialog(app_data);
    }
    else
    {
        new_file(app_data);
    }

    free(buffered_file_content);
}

void init_file_menu(GtkApplication* const app, struct AppData* const app_data)
{
    const GActionEntry action_entries[1] = {
        { "file-new", file_new, NULL, NULL, NULL }
    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), action_entries, G_N_ELEMENTS(action_entries), app_data);
}

void new_file(struct AppData* const app_data)
{
    app_data->file_name = "";
    app_data->file_path = "";
    app_data->file_content = "";
    gtk_text_buffer_set_text(app_data->text_buffer, "", 0);
}