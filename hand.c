#include <stdio.h>
#include <stdlib.h>
#include "hand.h"


/**
 * @brief Initializes a new hand.
 *
 * Creates an empty hand with no cards, a score of zero, and no allocated
 * memory for the card array.
 *
 * @return A newly initialized Hand structure.
 */
Hand init_hand(void) {
    Hand h;
    h.cards = NULL;
    h.count = 0;
    h.score = 0;
    return h;
}

/**
 * @brief Recalculates the score of a hand.
 *
 * Computes the total value of all cards in the hand, treating face cards as
 * 10 and adjusting the value of Aces between 11 and 1 when necessary to
 * prevent the hand from busting.
 *
 * @param h Pointer to the hand whose score will be updated.
 */
static void recalc_score(Hand* h) {
    int total = 0;
    int aceCount = 0;

    for (int i = 0; i < h->count; i++) {
        int rank = h->cards[i].rank; // assume 1 = Ace, 11 = J, 12 = Q, 13 = K

        if (rank == 1) {            // Ace
            total += 11;
            aceCount++;
        }
        else if (rank >= 11 && rank <= 13) { // face cards
            total += 10;
        }
        else {
            total += rank;          // 2 - 10
        }
    }

    // Ace adjustment: reduce 11 -> 1 while total > 21 and we still have Aces counted as 11
    while (total > 21 && aceCount > 0) {
        total -= 10; // 11 -> 1
        aceCount--;
    }

    h->score = total;
}

/**
 * @brief Adds a card to a hand.
 *
 * Resizes the hand's card array, appends the specified card, increments the
 * card count, and recalculates the hand's score.
 *
 * @param h Pointer to the hand receiving the card.
 * @param c The card to add to the hand.
 */
void add_card(Hand* h, Card c) {
    Card* newArray = (Card*)realloc(h->cards, (h->count + 1) * sizeof(Card));
    if (newArray == NULL) {
        printf("Memory allocation failed in add_card()\n");
        return;
    }

    h->cards = newArray;
    h->cards[h->count] = c;
    h->count++;

    recalc_score(h);
}

/**
 * @brief Recalculates the score of a hand.
 *
 * Computes the total value of all cards in the hand, treating face cards as
 * 10 and adjusting the value of Aces between 11 and 1 when necessary to
 * prevent the hand from busting.
 *
 * @param h Pointer to the hand whose score will be updated.
 */
static void recalc_score(Hand* h) {
    if (h->count == 0) {
        printf("Warning: get_score() called on empty hand.\n");
        return 0;
    }
    recalc_score(h);
    return h->score;
}


/**
 * @brief Determines whether a hand is a natural blackjack.
 *
 * A hand is considered a blackjack if it contains exactly two cards with a
 * total value of 21.
 *
 * @param h Pointer to the hand.
 * @return 1 if the hand is a blackjack, otherwise 0.
 */
int is_blackjack(Hand* h) {
    if (h->count == 2) {
        recalc_score(h);
        if (h->score == 21) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Frees all memory associated with a hand.
 *
 * Releases the dynamically allocated card array and resets the hand's fields
 * to their default values.
 *
 * @param h Pointer to the hand to be cleaned up.
 */
void free_hand(Hand* h) {
    if (h->cards != NULL) {
        free(h->cards);
        h->cards = NULL;
    }
    h->count = 0;
    h->score = 0;
}