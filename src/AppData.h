#pragma once

#include <gtk/gtk.h>

struct AppData
{
    GtkWindow* window;
    GtkTextBuffer* text_buffer;
    const gchar* file_name;
    const gchar* file_path;
    const gchar* file_content;
};