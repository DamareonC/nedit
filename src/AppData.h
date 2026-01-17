#pragma once

#include <gtk/gtk.h>

enum UnsavedType
{
    NONE = -1, CLOSE, NEW, OPEN
};

struct AppData
{
    GtkWindow* window;
    GtkTextBuffer* text_buffer;
    char* file_name;
    char* file_path;
    char* file_content;
    enum UnsavedType unsaved_type;
};

void set_app_data(struct AppData* const app_data, char* const file_name, char* const file_path, char* const file_content);