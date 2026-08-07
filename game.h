#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "deck.h"
#include "hand.h"

/**
 * @brief Plays one complete round of Blackjack.
 *
 * @param players Array of players.
 * @param num_players Number of players.
 * @param deck Pointer to the deck.
 */
void play_round(Player* players, int num_players, Deck* deck);

/**
 * @brief Executes a player's turn.
 *
 * @param p Pointer to the player.
 * @param playerHand Pointer to the player's hand.
 * @param dealerHand Pointer to the dealer's hand.
 * @param deck Pointer to the deck.
 */
void play_player_turn(Player* p, Hand* playerHand, Hand* dealerHand, Deck* deck);

/**
 * @brief Executes the dealer's turn.
 *
 * @param dealerHand Pointer to the dealer's hand.
 * @param deck Pointer to the deck.
 * @param player Pointer to the player being displayed.
 * @param playerHand Pointer to the player's hand.
 */
void play_dealer_turn(Hand* dealerHand, Deck* deck, Player* player, Hand* playerHand);

/**
 * @brief Determines the outcome of the round.
 *
 * @param players Array of players.
 * @param num_players Number of players.
 * @param playerHands Array of player hands.
 * @param dealerHand Pointer to the dealer's hand.
 */
void resolve_round(Player* players, int num_players, Hand* playerHands, Hand* dealerHand);

/**
 * @brief Handles the player's double down action.
 *
 * @param p Pointer to the player.
 * @param playerHand Pointer to the player's hand.
 * @param deck Pointer to the deck.
 */
void handle_double_down(Player* p, Hand* playerHand, Deck* deck);

#endif
