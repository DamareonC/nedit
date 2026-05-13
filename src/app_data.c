#include "app_data.h"

static void set_data(char** data, char* const string)
{
    g_free(*data);
    *data = string;
}

void set_file_data(char* const file_name, char* const file_path, char* const file_content, struct AppData* const app_data)
{
    set_data(&app_data->file_name, file_name);
    set_data(&app_data->file_path, file_path);
    set_data(&app_data->file_content, file_content);
}

void set_window_title(char* const window_title, struct AppData* const app_data)
{
    set_data(&app_data->window_title, window_title);
    gtk_window_set_title(app_data->window, app_data->window_title);
}