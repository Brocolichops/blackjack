#ifndef FILEIO_H
#define FILEIO_H

#include "player.h"

/**
 * @brief Loads a player profile by username.
 * @param username The username to search for.
 * @return Pointer to Player if found, otherwise NULL.
 */
Player* load_player(const char* username);

/**
 * @brief Saves the player's current data to storage.
 * @param player The player to save.
 * @return 1 on success, otherwise 0
 */
int save_player(Player* player);

/**
 * @brief Authenticates a user with username and password.
 * @param username The username.
 * @param password The password.
 * @return 1 if valid, 0 if invalid.
 */
int authenticate_user(const char* username, const char* password);

/**
 * @brief Creates a new player profile.
 * @param username The desired username.
 * @param password The chosen password.
 * @param startingChips Initial chip balance.
 * @return 1 on success, otherwise 0
 */
int create_profile(const char* username, const char* password, int startingChips);

/**
 * @brief Updates an existing player profile.
 * @param player The player whose data should be updated.
 * @return 1 on success, otherwise 0
 */
int update_profile(Player* player);

/**
 * @brief Deletes a player profile.
 * @param username The username to remove.
 * @return 1 on success, otherwise 0
 */
int delete_profile(const char* username);

#endif
