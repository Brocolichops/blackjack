#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "fileio.h"
#include "deck.h"
#include "hand.h"
#include "ui.h"
#include "game.h"

/**
 * @brief Handles the login/create-profile flow and sets up a fresh deck.
 *
 * Prompts the user through the main menu, either logging in an existing
 * player or creating a new profile, then initializes and shuffles the deck.
 *
 * @param playerOut Output pointer that will be set to the loaded/created player.
 * @param deck Pointer to the deck to initialize for the game.
 * @return 0 on success, -1 on failure (invalid login, failed profile creation, etc.).
 */
int initializeGame(Player** playerOut, Deck* deck) {
    srand((unsigned int)time(NULL));

    display_menu(MENU_MAIN);
    int choice = get_validated_choice(1, 3);

    char username[50];
    char password[50];

    if (choice == 1) {
        // LOGIN
        display_menu(MENU_LOGIN);
        scanf_s("%49s", username, (unsigned)_countof(username));

        printf("Enter password: ");
        scanf_s("%49s", password, (unsigned)_countof(password));

        if (!authenticate_user(username, password)) {
            display_message("Invalid username or password.", MSG_ERROR);
            return -1;
        }

        Player* p = load_player(username);
        if (p == NULL) {
            display_message("Failed to load profile.", MSG_ERROR);
            return -1;
        }

        *playerOut = p;
    }
    else if (choice == 2) {
        // CREATE PROFILE
        printf("Choose a username: ");
        scanf_s("%49s", username, (unsigned)_countof(username));

        printf("Choose a password: ");
        scanf_s("%49s", password, (unsigned)_countof(password));

        int startingChips = 200; // default starting chips

        if (!create_profile(username, password, startingChips)) {
            display_message("Failed to create profile.", MSG_ERROR);
            return -1;
        }

        Player* p = load_player(username);
        if (p == NULL) {
            display_message("Failed to load new profile.", MSG_ERROR);
            return -1;
        }

        *playerOut = p;
    }
    else {
        display_message("Goodbye!", MSG_INFO);
        exit(0);
    }

    // Initialize deck
    if (initDeck(deck) != 0) {
        display_message("Deck initialization failed.", MSG_ERROR);
        return -1;
    }

    shuffleDeck(deck);
    return 0;
}

/**
 * @brief Saves the player's profile and frees all game resources.
 * @param player Pointer to the player to save and free (may be NULL).
 * @param deck Pointer to the deck to free.
 */
void cleanupGame(Player* player, Deck* deck) {
    if (player != NULL) {
        update_profile(player);
        free(player);
    }
    freeDeck(deck);
}

/**
 * @brief Program entry point. Runs the login flow, then the main betting
 *        and round loop until the player quits or runs out of chips.
 * @param argc Argument count (unused).
 * @param argv Argument vector (unused).
 * @return 0 on normal exit, -1 on initialization failure.
 */
int main(int argc, char* argv[]) {
    Player* player = NULL;
    Deck deck;

    if (initializeGame(&player, &deck) != 0) {
        display_message("Game initialization failed.", MSG_ERROR);
        return -1;
    }

    while (1) {
        display_menu(MENU_BET);

        int bet;
        scanf_s("%d", &bet);

        if (!update_bet(player, bet)) {
            display_message("Invalid bet amount.", MSG_WARNING);
            continue;
        }

        update_chips(player, -bet); // deduct initial bet

        play_round(player, 1, &deck);

        if (player->chipBalance <= 0) {
            display_message("You are out of chips!", MSG_RESULT);
            break;
        }

        printf("Play another round? (1 = yes, 0 = no): ");
        int again;
        scanf_s("%d", &again);

        if (!again) {
            break;
        }

        resetDeck(&deck);
    }

    cleanupGame(player, &deck);
    display_message("Thanks for playing Blackjack!", MSG_INFO);

    return 0;
}