#pragma once

#include "app_data.h"

void init_file_menu(GtkApplication* const app, struct AppData* const app_data);
void new_file(struct AppData* const app_data);
void open_file(GFile* const file, struct AppData* const app_data);
bool save_file(GFile* const file, const struct AppData* const app_data);
bool save_as_file(GFile* const file, struct AppData* const app_data);
bool is_unsaved(const struct AppData* const app_data);
char* get_buffer_text(const struct AppData* const app_data);