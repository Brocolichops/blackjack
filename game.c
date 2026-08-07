#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "ui.h"
#include "fileio.h"
#include "player.h"
#include "deck.h"
#include "hand.h"

/**
 * @brief Handles the player's double down action.
 *
 * Doubles the player's current bet if they have enough chips, deducts the
 * additional bet from their chip balance, deals exactly one additional card,
 * and checks whether the player has busted.
 *
 * @param p Pointer to the player performing the double down.
 * @param playerHand Pointer to the player's hand.
 * @param deck Pointer to the deck used for dealing cards.
 */
void handle_double_down(Player* p, Hand* playerHand, Deck* deck) {
    int originalBet = p->currentBet;
    int extraBet = originalBet;

    if (!update_bet(p, originalBet * 2)) {
        display_message("Not enough chips to double down.", MSG_ERROR);
        return;
    }

    update_chips(p, -extraBet);

    Card c = dealCard(deck);
    add_card(playerHand, c);

    int score = get_score(playerHand);
    if (score > 21) {
        display_message("You busted after double down!", MSG_RESULT);
    }
}

/**
 * @brief Executes a player's turn.
 *
 * Repeatedly displays the current game state and allows the player to choose
 * an action such as hit, stand, double down, or quit. The turn ends when the
 * player stands, busts, doubles down, or quits the round.
 *
 * @param p Pointer to the current player.
 * @param playerHand Pointer to the player's hand.
 * @param dealerHand Pointer to the dealer's hand.
 * @param deck Pointer to the deck used for dealing cards.
 */
void play_player_turn(Player* p, Hand* playerHand, Hand* dealerHand, Deck* deck) {
    int allowDoubleDown = (playerHand->count == 2);
    int done = 0;

    while (!done) {


        display_game_state(p, playerHand, dealerHand, 1);

        int action = get_player_actiona(allowDoubleDown);

        switch (action) {

        case 1: {
            Card c = dealCard(deck);
            add_card(playerHand, c);

            int score = get_score(playerHand);
            if (score > 21) {
                display_message("You busted!", MSG_RESULT);
                done = 1;
            }
            break;
        }

        case 2: 
            done = 1;
            break;

        case 3:
            if (allowDoubleDown) {
                handle_double_down(p, playerHand, deck);
                done = 1;
            }
            else {
                display_message("Double Down is only allowed on your first action.", MSG_WARNING);
            }
            break;

        case 4: 
            display_message("You chose to quit the round.", MSG_INFO);
            done = 1;
            break;

        default:
            display_message("Invalid action.", MSG_ERROR);
            break;
        }

        allowDoubleDown = 0;
    }
}

/**
 * @brief Executes the dealer's turn.
 *
 * Reveals the dealer's hidden card and continues drawing cards until the
 * dealer's hand reaches a score of at least 17, displaying the updated game
 * state after each draw.
 *
 * @param dealerHand Pointer to the dealer's hand.
 * @param deck Pointer to the deck used for dealing cards.
 * @param player Pointer to the player whose game state is displayed.
 * @param playerHand Pointer to the player's hand.
 */
void play_dealer_turn(Hand* dealerHand, Deck* deck, Player* player, Hand* playerHand) {
    // Reveal dealer's hidden card
    display_message("Dealer reveals their hidden card...", MSG_INFO);
    display_game_state(player, playerHand, dealerHand, 0);

    int score = get_score(dealerHand);

    // Dealer draws until 17+
    while (score < 17) {
        display_message("Dealer draws a card...", MSG_INFO);

        Card c = dealCard(deck);
        add_card(dealerHand, c);

        display_game_state(player, playerHand, dealerHand, 0);

        score = get_score(dealerHand);
    }
}

/**
 * @brief Determines the outcome of a completed round for all players.
 *
 * Compares each player's final hand against the dealer's hand, determines
 * whether the player wins, loses, pushes, or has blackjack, updates chip
 * balances and player statistics, displays the round results, and saves the
 * updated player profile.
 *
 * @param players Array of players participating in the round.
 * @param num_players Number of players in the array.
 * @param playerHands Array containing each player's hand.
 * @param dealerHand Pointer to the dealer's hand.
 */
void resolve_round(Player* players, int num_players, Hand* playerHands, Hand* dealerHand) {
    int dealerScore = get_score(dealerHand);
    int dealerBust = (dealerScore > 21);

    for (int i = 0; i < num_players; i++) {
        Player* p = &players[i];
        Hand* h = &playerHands[i];

        int playerScore = get_score(h);
        int playerBust = (playerScore > 21);
        int roundResult = -1;
        int chipsDelta = 0;

        if (playerBust) {
            roundResult = 0; // loss
            chipsDelta = -p->currentBet;
            update_chips(p, chipsDelta);
        }
        else if (dealerBust) {
            // dealer bust, player wins
            roundResult = 1;
            chipsDelta = p->currentBet; // win equal to bet
            update_chips(p, chipsDelta);
        }
        else if (is_blackjack(h)) {
            // natural blackjack
            roundResult = 3;
            chipsDelta = (int)(p->currentBet * 1.5);
            update_chips(p, chipsDelta);
        }
        else if (playerScore > dealerScore) {
            roundResult = 1;
            chipsDelta = p->currentBet;
            update_chips(p, chipsDelta);
        }
        else if (playerScore < dealerScore) {
            roundResult = 0;
            chipsDelta = -p->currentBet;
            update_chips(p, chipsDelta);
        }
        else {
            roundResult = 2; // push
            chipsDelta = 0;
        }

        update_statistics(p, (roundResult == 1 || roundResult == 3) ? 1 : 0);
        display_round_result(roundResult, chipsDelta, p, h, dealerHand);
        update_profile(p); // save stats
    }
}

/**
 * @brief Plays a complete round of Blackjack.
 *
 * Allocates memory for player hands, deals the initial cards, manages each
 * player's turn, executes the dealer's turn, determines the round results,
 * and releases all allocated memory before returning.
 *
 * @param players Array of players participating in the game.
 * @param num_players Number of players in the array.
 * @param deck Pointer to the deck used during the round.
 */
void play_round(Player* players, int num_players, Deck* deck) {
    Hand* playerHands = (Hand*)malloc(num_players * sizeof(Hand));
    if (playerHands == NULL) {
        display_message("Memory allocation failed in play_round().", MSG_ERROR);
        return;
    }

    for (int i = 0; i < num_players; i++) {
        playerHands[i] = init_hand();
    }
    Hand dealerHand = init_hand();

    // initial deal: 2 cards to each player, 2 to dealer
    for (int i = 0; i < num_players; i++) {
        add_card(&playerHands[i], dealCard(deck));
        add_card(&playerHands[i], dealCard(deck));
    }
    add_card(&dealerHand, dealCard(deck)); // visible
    add_card(&dealerHand, dealCard(deck)); // hidden

    // each player's turn
    for (int i = 0; i < num_players; i++) {
        play_player_turn(&players[i], &playerHands[i], &dealerHand, deck);
    }

    // dealer's turn (for now, show relative to first player)
    play_dealer_turn(&dealerHand, deck, &players[0], &playerHands[0]);

    // resolve round
    resolve_round(players, num_players, playerHands, &dealerHand);

    // cleanup
    for (int i = 0; i < num_players; i++) {
        free_hand(&playerHands[i]);
    }
    free(playerHands);
    free_hand(&dealerHand);
}
