#pragma once 
#include <string.h>

typedef struct Player {
	char username[32];
	char password[16];
	int chipBalance;
	int wins;
	int loss;
	int gamesPlayed;
} Player;


Player* init_player(char* username, int startingChips);
int update_bet(Player* player, int betAmount);
void update_chips(Player* player, int chipChange);
void update_statistics(Player* player, int roundResult);

