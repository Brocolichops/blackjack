#include <stdio.h>
#include "player.h"
#include <stdlib.h>

Player* init_player(char* username, int startingChips) {
	//Creating a pointer variable to the struct and dynamically allocating memory.
	Player* player = (Player*)malloc(sizeof(Player));

	if (player == NULL) {
		//Verifying memory allocation.
		printf("Memory allocation failed at: init_player()");
		return NULL;
	}

	printf("Enter your password: ");
	//Reading the first 16 characters entered by the user.
	scanf_s("%16s", 16, player->password);

	//Populating the struct with default values
	strncpy_s(player->username, sizeof(player->username), username, _TRUNCATE);
	player->chipBalance = startingChips;
	player->wins = 0;
	player->loss = 0;
	player->gamesPlayed = 0;

	//Returning the struct.
	return player;
}

int update_bet(Player* player, int betAmount) {
	//Condition to play bet. If player has insufficient funds, reject the bet. Else, accept.
	if (betAmount > player->chipBalance) {
		return 0;
	}
	else {
		return 1;
	}
}

void update_chips(Player* player, int chipChange) {
	player->chipBalance -= chipChange;
}

void update_statistics(Player* player, int roundResult) {
	if (roundResult == 1) {
		player->wins++;
		player->loss--;
	}
	else {
		player->wins--;
		player->loss--;
	}
	player->gamesPlayed++;
}
