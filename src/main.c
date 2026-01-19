#include "main.h"

int main(int argc, char* argv[])
{
    struct AppData app_data = {
        .file_name = g_malloc0(sizeof(char)),
        .file_path = g_malloc0(sizeof(char)),
        .file_content = g_malloc0(sizeof(char)),
        .unsaved_type = NONE
    };

    GtkApplication* const app = gtk_application_new("nedit.app", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(start), &app_data);
    g_signal_connect(app, "open", G_CALLBACK(start_with_files), &app_data);
    const int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    g_free(app_data.file_name);
    g_free(app_data.file_path);
    g_free(app_data.file_content);
    return status;
}