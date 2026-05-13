#pragma once

#include <gtk/gtk.h>

enum UnsavedType
{
    NONE = -1,
    CLOSE,
    NEW,
    OPEN
};

struct AppData
{
    GtkWindow* window;
    GtkTextBuffer* text_buffer;
    char* window_title;
    char* file_name;
    char* file_path;
    char* file_content;
    enum UnsavedType unsaved_type;
};

void set_file_data(char* file_name, char* file_path, char* file_content, struct AppData* app_data);
void set_window_title(char* window_title, struct AppData* app_data);