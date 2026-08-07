#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "deck.h"
#include "hand.h"
#include "player.h"

static void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#define CARD_WIDTH  7   // interior display width between the side borders
#define CARD_HEIGHT 5   // number of interior rows
#define CARD_BUF (CARD_WIDTH + 3)

// Returns the display rank ("A","2"..."10","J","Q","K") for a card rank.
static const char* rank_label(int rank) {
    static const char* ranks[] = {
        "?", "A", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "J", "Q", "K"
    };
    if (rank < 1 || rank > 13) return "?";
    return ranks[rank];
}

// Returns the suit glyph as a single-byte codepage-437 (OEM/DOS) character.
// The default Windows console starts in codepage 437, which natively maps
// these byte values to the card-suit glyphs (no console setup required).
static char suit_symbol(int suit) {
    switch (suit) {
    case 0: return 'H'; // Hearts
    case 1: return 'D'; // Diamonds
    case 2: return 'C'; // Clubs
    case 3: return 'S'; // Spades
    default: return '?';
    }
}

// Fills 'lines' with CARD_HEIGHT+2 rows (top border, interior, bottom border)
// of an ASCII-art playing card, each row null-terminated.
static void build_card_lines(Card c, char lines[][CARD_BUF]) {
    const char* rank = rank_label(c.rank);
    char suit = suit_symbol(c.suit);
    int row = 0;

    // top border
    snprintf(lines[row++], CARD_BUF, "+-------+");

    // rank in top-left
    snprintf(lines[row++], CARD_BUF, "|%-2s     |", rank);

    // blank
    snprintf(lines[row++], CARD_BUF, "|       |");

    // suit centered
    snprintf(lines[row++], CARD_BUF, "|   %c   |", suit);

    // blank
    snprintf(lines[row++], CARD_BUF, "|       |");

    // rank in bottom-right
    snprintf(lines[row++], CARD_BUF, "|     %2s|", rank);

    // bottom border
    snprintf(lines[row++], CARD_BUF, "+-------+");
}

// Fills 'lines' with the same shape as build_card_lines(), but drawn as a
// face-down card back (used for the dealer's hidden card).
static void build_hidden_card_lines(char lines[][CARD_BUF]) {
    int row = 0;
    snprintf(lines[row++], CARD_BUF, "+-------+");
    snprintf(lines[row++], CARD_BUF, "|# # # #|");
    snprintf(lines[row++], CARD_BUF, "| # # # |");
    snprintf(lines[row++], CARD_BUF, "|# # # #|");
    snprintf(lines[row++], CARD_BUF, "| # # # |");
    snprintf(lines[row++], CARD_BUF, "|# # # #|");
    snprintf(lines[row++], CARD_BUF, "+-------+");
}

#define CARD_ROWS (CARD_HEIGHT + 2)

// Prints an entire hand of cards side-by-side as ASCII art.
// If hideFirst is non-zero, the first card is drawn as a face-down card back.
static void display_hand_ascii(const Hand* hand, int hideFirst) {
    if (hand == NULL || hand->count == 0) {
        printf("(no cards)\n");
        return;
    }

    char (*allLines)[CARD_ROWS][CARD_BUF] =
        malloc(hand->count * sizeof(*allLines));
    if (allLines == NULL) {
        printf("Memory allocation failed in display_hand_ascii()\n");
        return;
    }

    for (int i = 0; i < hand->count; i++) {
        if (i == 0 && hideFirst) {
            build_hidden_card_lines(allLines[i]);
        }
        else {
            build_card_lines(hand->cards[i], allLines[i]);
        }
    }

    for (int row = 0; row < CARD_ROWS; row++) {
        for (int i = 0; i < hand->count; i++) {
            printf("%s ", allLines[i][row]);
        }
        printf("\n");
    }

    free(allLines);
}

void display_menu(int menuType) {
    clear_screen();

    switch (menuType) {
    case MENU_MAIN:
        printf("=== BLACKJACK 21 ===\n");
        printf("1. Login\n");
        printf("2. Create Profile\n");
        printf("3. Quit\n");
        break;

    case MENU_LOGIN:
        printf("=== LOGIN ===\n");
        printf("Enter your username:\n");
        break;

    case MENU_ACTION:
        printf("=== YOUR TURN ===\n");
        printf("1. Hit\n");
        printf("2. Stand\n");
        printf("3. Double Down\n");
        printf("4. Quit Game\n");
        break;

    case MENU_BET:
        printf("=== PLACE YOUR BET ===\n");
        printf("Enter your bet amount:\n");
        break;

    default:
        printf("Unknown menu.\n");
        break;
    }
}

int get_validated_choice(int minChoice, int maxChoice) {
    int choice;
    while (1) {
        printf("Enter choice (%d-%d): ", minChoice, maxChoice);
        if (scanf_s("%d", &choice) != 1) {
            while (getchar() != '\n'); // flush
            display_message("Invalid input. Try again.", MSG_ERROR);
            continue;
        }
        if (choice < minChoice || choice > maxChoice) {
            display_message("Choice out of range.", MSG_WARNING);
            continue;
        }
        return choice;
    }
}

int get_player_actiona(int allowDoubleDown) {
    printf("Choose your action:\n");
    printf("1. Hit\n");
    printf("2. Stand\n");

    if (allowDoubleDown) {
        printf("3. Double Down\n");
        printf("4. Quit\n");
        return get_validated_choice(1, 4);
    }
    else {
        printf("3. Quit\n");
        return get_validated_choice(1, 3);
    }
}

void display_game_state(const Player* player,
    const Hand* playerHand,
    const Hand* dealerHand,
    int hideDealerCard)
{
    clear_screen();

    printf("=== GAME STATE ===\n\n");

    if (player != NULL) {
        printf("Player: %s\n", player->username);
        printf("Chips: %d\n", player->chipBalance);
        printf("Current Bet: %d\n\n", player->currentBet);

        printf("Your Hand:\n");
        display_hand_ascii(playerHand, 0);
        printf("Score: %d\n\n", get_score(playerHand));
    }

    printf("Dealer Hand:\n");
    display_hand_ascii(dealerHand, hideDealerCard);
    if (hideDealerCard)
        printf("Score: ??\n\n");
    else
        printf("Score: %d\n\n", get_score(dealerHand));
}

void display_message(const char* message, int messageType) {
    switch (messageType) {
    case MSG_INFO:
        printf("[INFO] %s\n", message);
        break;
    case MSG_ERROR:
        printf("[ERROR] %s\n", message);
        break;
    case MSG_WARNING:
        printf("[WARNING] %s\n", message);
        break;
    case MSG_RESULT:
        printf("[RESULT] %s\n", message);
        break;
    default:
        printf("%s\n", message);
        break;
    }
}

void display_round_result(int roundResult,
    int chipsWonOrLost,
    const Player* player,
    const Hand* playerHand,
    const Hand* dealerHand)
{
    clear_screen();

    printf("=== ROUND RESULT ===\n\n");

    printf("Your Final Hand:\n");
    if (playerHand != NULL) {
        display_hand_ascii(playerHand, 0);
        printf("Score: %d\n\n", get_score(playerHand));
    }

    printf("Dealer Final Hand:\n");
    if (dealerHand != NULL) {
        display_hand_ascii(dealerHand, 0);
        printf("Score: %d\n\n", get_score(dealerHand));
    }

    switch (roundResult) {
    case 1:
        printf("You WIN!\n");
        printf("Chips Won: +%d\n", chipsWonOrLost);
        break;
    case 0:
        printf("You LOSE!\n");
        printf("Chips Lost: -%d\n", chipsWonOrLost);
        break;
    case 2:
        printf("Push (Tie)\n");
        printf("Your bet was returned.\n");
        break;
    case 3:
        printf("Blackjack! You win 1.5x your bet!\n");
        printf("Chips Won: +%d\n", chipsWonOrLost);
        break;
    default:
        printf("Unknown round result.\n");
        break;
    }

    printf("\nYour new chip balance: %d\n", player->chipBalance);
    printf("Press Enter to continue...");
    getchar();
    getchar();
}