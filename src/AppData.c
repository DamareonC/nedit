#include "AppData.h"

static void set_data(char** data, char* const string)
{
    g_free(*data);
    *data = string;
}

void set_app_data(char* const file_name, char* const file_path, char* const file_content, struct AppData* const app_data)
{
    set_data(&app_data->file_name, file_name);
    set_data(&app_data->file_path, file_path);
    set_data(&app_data->file_content, file_content);
}