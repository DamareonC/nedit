#pragma once

#include "AppData.h"

void init_file_menu(GtkApplication* const app, struct AppData* const app_data);
void new_file(struct AppData* const app_data);
void open_file(GFile* const file, struct AppData* const app_data);
bool save_file(const struct AppData* const app_data);
bool is_unsaved(const struct AppData* const app_data);