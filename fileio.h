#pragma once
#include "player.h"


extern const char FILE_NAME;

Player* load_player(const char* username);
int save_player(Player* player);
int authenticate_user(const char* username, const char* password);
int create_profile(const char* username, const char* password, int startingChips);
void update_profile(Player* player);
int delete_profile(const char* username);