#pragma once

#include "app_data.h"

void init_file_menu(GtkApplication* app, struct AppData* app_data);
void new_file(struct AppData* app_data);
void open_file(GFile* file, struct AppData* app_data);
bool save_file(GFile* file, const struct AppData* app_data);
bool save_as_file(GFile* file, struct AppData* app_data);
bool is_unsaved(const struct AppData* app_data);
char* get_buffer_text(const struct AppData* app_data);