#ifndef UI_H
#define UI_H

#include "player.h"
#include "hand.h"

// Message types
#define MSG_INFO    1
#define MSG_ERROR   2
#define MSG_WARNING 3
#define MSG_RESULT  4

// Menu types
#define MENU_MAIN   1
#define MENU_LOGIN  2
#define MENU_ACTION 3
#define MENU_BET    4

void display_menu(int menuType);
int get_validated_choice(int minChoice, int maxChoice);

void display_game_state(const Player* player,
    const Hand* playerHand,
    const Hand* dealerHand,
    int hideDealerCard);

int get_player_actiona(int allowDoubleDown);

void display_message(const char* message, int messageType);

void display_round_result(int roundResult,
    int chipsWonOrLost,
    const Player* player,
    const Hand* playerHand,
    const Hand* dealerHand);

#endif
