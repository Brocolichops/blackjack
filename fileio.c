#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "fileio.h"

#define FILE_NAME "playerData.txt"

Player* load_player(const char* username);
int save_player(Player* player);
int authenticate_user(const char* username, const char* password);
int create_profile(const char* username, const char* password, int startingChips);
void update_profile(Player* player);
int delete_profile(const char* username);

Player* load_player(const char* username) {
	Player* player = (Player*)malloc(sizeof(Player));

	if (player == NULL) {
		printf("Memory allocation error at: load_player()");
		return NULL;
	}

	FILE* fptr;
	
	if (fopen_s(&fptr, FILE_NAME, "r") != 0) {
		printf("File failed to open at: load_player()");
		return NULL;
	}

	char line[50];
	while (fgets(line, 50, fptr)) {
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, username) == 0) {
			strcpy_s(player->username, sizeof(player->username), line);

			fgets(line, 50, fptr);
			line[strcspn(line, "\n")] = '\0';
			strcpy_s(player->password, sizeof(player->password), line);

			fgets(line, 50, fptr);
			line[strcspn(line, "\n")] = '\0';
			player->chipBalance = atoi(line);

			fgets(line, 50, fptr);
			line[strcspn(line, "\n")] = '\0';
			player->wins = atoi(line);

			fgets(line, 50, fptr);
			line[strcspn(line, "\n")] = '\0';
			player->loss = atoi(line);

			fgets(line, 50, fptr);
			line[strcspn(line, "\n")] = '\0';
			player->gamesPlayed = atoi(line);
			
			fgets(line, 50, fptr);

			fclose(fptr);
			return player;
		}

		for (int i = 0; i < 5; i++) {
			fgets(line, 50, fptr);
		}
	}
	fclose(fptr);
	free(player);
	printf("Username not found!");
	return NULL;
}

int save_player(Player* player) {
	FILE* fptr;

	if (fopen_s(&fptr, FILE_NAME, "a") != 0) {
		printf("Failed to open file at: save_player()");
		return 0;
	}

	fprintf(fptr, "\n\n%s\n%s\n%d\n%d\n%d\n%d", player->username, player->password, player->chipBalance, player->wins, player->loss, player->gamesPlayed);

	fclose(fptr);
	return 1;
}

int authenticate_user(const char* username, const char* password) {
	if (strcmp(username, password) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

int create_profile(const char* username, const char* password, int startingChips) {
	FILE* fptr;

	if (fopen_s(&fptr, FILE_NAME, "a+") != 0) {
		printf("Failed to open file at: create_profile()\n");
		return 0;
	}
	char line[50];
	while (fgets(line, 50, fptr)) {
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, username) == 0) {
			printf("Username already exists!\n");
			return 0;
		}
	}

	fprintf(fptr, "\n%s\n%s\n%d\n%d\n%d\n%d", username, password, startingChips, 0, 0, 0);
	fclose(fptr);
}

void update_profile(Player* player) {
	FILE* fptr = NULL;
	FILE* tempFp = NULL;

	if (fopen_s(&fptr, FILE_NAME, "r") != 0 || fopen_s(&tempFp, "temp.txt", "w") != 0) {
		printf("Failed to open file at: update_profile()\n");
		return;
	}

	char line[50];

	while (fgets(line, sizeof(line), fptr)) {
		line[strcspn(line, "\n")] = '\0';

		if (strcmp(player->username, line) == 0) {

			fprintf(tempFp, "%s\n", player->username);
			fprintf(tempFp, "%s\n", player->password);
			fprintf(tempFp, "%d\n", player->chipBalance);
			fprintf(tempFp, "%d\n", player->wins);
			fprintf(tempFp, "%d\n", player->loss);
			fprintf(tempFp, "%d\n", player->gamesPlayed);

			for (int i = 0; i < 6; i++) {
				fgets(line, sizeof(line), fptr);
			}
		}
		else {

			fprintf(tempFp, "%s\n", line);
		}
	}

	fclose(fptr);
	fclose(tempFp);

	if (fopen_s(&fptr, FILE_NAME, "w") != 0 || fopen_s(&tempFp, "temp.txt", "r") != 0) {
		printf("Failed to reopen files.\n");
		return;
	}

	while (fgets(line, sizeof(line), tempFp)) {
		fprintf(fptr, "%s", line);
	}

	fclose(fptr);
	fclose(tempFp);

	remove("temp.txt");

	return 1;
}

int delete_profile(const char* username) {
	FILE* fptr;
	FILE* tempFp;

	if (fopen_s(&fptr, FILE_NAME, "r") != 0 || fopen_s(&tempFp, "temp.txt", "w") != 0) {
		printf("Failed to open file at: delete_profile()");
		return 0;
	}
	char line[50];
	bool userFound = false;
	while (fgets(line, 50, fptr)) {
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, username) == 0) {
			userFound = true;
			for (int i = 0; i < 5; i++) {
				fgets(line, 50, fptr);
			}
		}
		else {
			fprintf(tempFp, "%s\n", line);

		}
	};

	fclose(fptr);
	fclose(tempFp);

	if (userFound == false) {
		printf("User not found!\n");
		return 0;
	}

	if (fopen_s(&fptr, FILE_NAME, "w") != 0 || fopen_s(&tempFp, "temp.txt", "r") != 0){
		printf("Failed to open file at: delete_profile()");
		return 0;
	}

	while (fgets(line, 50, tempFp)) {
		fprintf(fptr, "%s", line);
	}

	fclose(fptr);
	fclose(tempFp);

	remove("temp.txt");

	return 1;
}
