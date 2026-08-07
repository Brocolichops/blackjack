#ifndef HAND_H
#define HAND_H

#include "deck.h"   // for Card

typedef struct {
    Card* cards;   // pointer to dynamically allocated card array
    int count;     // number of cards currently in the hand
    int score;     // current calculated score
} Hand;

/**
 * @brief Creates and initializes an empty hand.
 *
 * @return A newly initialized Hand structure.
 */
Hand init_hand(void);

/**
 * @brief Adds a card to a hand.
 *
 * @param h Pointer to the hand.
 * @param c Card to add.
 */
void add_card(Hand* h, Card c);

/**
 * @brief Returns the score of a hand.
 *
 * @param h Pointer to the hand.
 * @return Current score.
 */
int get_score(Hand* h);

/**
 * @brief Determines whether a hand is a natural blackjack.
 *
 * @param h Pointer to the hand.
 * @return 1 if blackjack, otherwise 0.
 */
int is_blackjack(Hand* h);

/**
 * @brief Releases all memory used by a hand.
 *
 * @param h Pointer to the hand.
 */
void free_hand(Hand* h);

#endif
