#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"
#include "fileio.h"

#define FILE_NAME "playerData.txt" //Global define for the file name

/**
* @brief Loads the player information
* 
* This function opens the data file and searches for the username &
* loads their information into a player struct
* 
* @param username A pointer to a string that holds the username of the player
* @return A pointer to a player struct.
*/
Player* load_player(const char* username) {
    //Dynamically allocating player struct pointer
    Player* player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Memory allocation error at: load_player()\n");
        return NULL;
    }

    //Creating file pointer
    FILE* fptr;
    if (fopen_s(&fptr, FILE_NAME, "r") != 0) {
        printf("File failed to open at: load_player()\n");
        free(player);
        return NULL;
    }
    //Buffer to hold read from file pointer
    char line[50];
    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0'; //Formatting read to exclude newline chars
        if (strcmp(line, username) == 0) {
            strcpy_s(player->username, sizeof(player->username), line);

            // password
            if (!fgets(line, sizeof(line), fptr)) break;
            line[strcspn(line, "\n")] = '\0';
            strcpy_s(player->password, sizeof(player->password), line);

            // chipBalance
            if (!fgets(line, sizeof(line), fptr)) break;
            line[strcspn(line, "\n")] = '\0';
            player->chipBalance = atoi(line);

            // wins
            if (!fgets(line, sizeof(line), fptr)) break;
            line[strcspn(line, "\n")] = '\0';
            player->wins = atoi(line);

            // loss
            if (!fgets(line, sizeof(line), fptr)) break;
            line[strcspn(line, "\n")] = '\0';
            player->loss = atoi(line);

            // gamesPlayed
            if (!fgets(line, sizeof(line), fptr)) break;
            line[strcspn(line, "\n")] = '\0';
            player->gamesPlayed = atoi(line);

            fclose(fptr);
            return player;
        }

        // Skipping rest of the profile
        for (int i = 0; i < 5; i++) {
            if (!fgets(line, sizeof(line), fptr)) {
                break;
            }
        }
    }

    fclose(fptr);
    free(player);
    printf("Username not found!\n");
    return NULL;
}

/**(
* @brief Function to save player information
* 
* This function takes the players' structs and saves the information into the
* text file. 
* 
* @param player A struct pointer to the player struct
* @return 0 if save failed, 1 if player was saved
*/

int save_player(Player* player) {
    //Opening file pointer
    FILE* fptr;
    if (fopen_s(&fptr, FILE_NAME, "a") != 0) {
        printf("Failed to open file at: save_player()\n");
        return 0;
    }

    //Writing the information into the file
    fprintf(fptr, "\n%s\n%s\n%d\n%d\n%d\n%d",
        player->username,
        player->password,
        player->chipBalance,
        player->wins,
        player->loss,
        player->gamesPlayed);

    fclose(fptr);
    return 1;
}

/**
* @brief Function to authenticate username and password entered by the user
* 
* This function takes the username and password and checks if they 
* match the ones entered in the data file
* 
* @param username A pointer to the username string
* @param password A pointer to the password string
* @return 0 if authentication failed, 1 if authentication was successfull
*/
int authenticate_user(const char* username, const char* password) {
    Player* player = load_player(username);
    if (player == NULL) {
        return 0;
    }

    int result = (strcmp(player->password, password) == 0) ? 1 : 0;
    free(player);
    return result;
}

/**
* @brief Function to create a new profile
* 
* This function creates a new profile for the user, 
* only if the username does not exist and is unique
* 
* @param username A pointer to the username string
* @param password A pointer to the password string
* @param startingChips Number of starting chips the user has
*/

int create_profile(const char* username, const char* password, int startingChips) {
    // Checking if user already exists
    Player* existing = load_player(username);
    if (existing != NULL) {
        printf("Username already exists!\n");
        free(existing);
        return 0;
    }

    Player* player = init_player(username, startingChips);
    if (player == NULL) {
        return 0;
    }
    strcpy_s(player->password, sizeof(player->password), password);

    int result = save_player(player);
    free(player);
    return result;
}

/**
* @brief Function to update the user profile
* 
* This function allows the user to change their information
* and updates it in the text file.
* 
* @param player A pointer to the player struct
* @return 0 if profile update failed, 1 if passed
*/
int update_profile(Player* player) {
    FILE* fptr = NULL;
    FILE* tempFp = NULL;

    if (fopen_s(&fptr, FILE_NAME, "r") != 0 || fopen_s(&tempFp, "temp.txt", "w") != 0) {
        printf("Failed to open file at: update_profile()\n");
        return 0;
    }

    char line[50];
    bool updated = false;

    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(player->username, line) == 0) {
            // overwrite this profile
            fprintf(tempFp, "%s\n", player->username);
            fprintf(tempFp, "%s\n", player->password);
            fprintf(tempFp, "%d\n", player->chipBalance);
            fprintf(tempFp, "%d\n", player->wins);
            fprintf(tempFp, "%d\n", player->loss);
            fprintf(tempFp, "%d\n", player->gamesPlayed);

            // skip old data
            for (int i = 0; i < 5; i++) {
                if (!fgets(line, sizeof(line), fptr)) break;
            }
            updated = true;
        }
        else {
            fprintf(tempFp, "%s\n", line);
        }
    }

    fclose(fptr);
    fclose(tempFp);

    if (!updated) {
        printf("User not found during update.\n");
        remove("temp.txt");
        return 0;
    }

    if (fopen_s(&fptr, FILE_NAME, "w") != 0 || fopen_s(&tempFp, "temp.txt", "r") != 0) {
        printf("Failed to reopen files.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), tempFp)) {
        fputs(line, fptr);
    }

    fclose(fptr);
    fclose(tempFp);
    remove("temp.txt");

    return 1;
}

/**
* @brief Function to delete an existing profile from the file
* 
* This function takes the username from the player and parses through the file,
* if the user is present it deletes the user.
* 
* @param username A pointer to the username string
* @return 0 if user was not found, 1 if user was deleted
*/

int delete_profile(const char* username) {
    FILE* fptr;
    FILE* tempFp;

    if (fopen_s(&fptr, FILE_NAME, "r") != 0 || fopen_s(&tempFp, "temp.txt", "w") != 0) {
        printf("Failed to open file at: delete_profile()\n");
        return 0;
    }

    char line[50];
    bool userFound = false;

    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, username) == 0) {
            userFound = true;
            // skip rest of this profile
            for (int i = 0; i < 5; i++) {
                if (!fgets(line, sizeof(line), fptr)) break;
            }
        }
        else {
            fprintf(tempFp, "%s\n", line);
        }
    }

    fclose(fptr);
    fclose(tempFp);

    if (!userFound) {
        printf("User not found!\n");
        remove("temp.txt");
        return 0;
    }

    if (fopen_s(&fptr, FILE_NAME, "w") != 0 || fopen_s(&tempFp, "temp.txt", "r") != 0) {
        printf("Failed to open file at: delete_profile()\n");
        return 0;
    }

    while (fgets(line, sizeof(line), tempFp)) {
        fputs(line, fptr);
    }

    fclose(fptr);
    fclose(tempFp);
    remove("temp.txt");

    return 1;
}
