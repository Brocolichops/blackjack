#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"

/**
 * @brief Allocates and fills a standard 52-card deck, then shuffles it.
 * @param deck Pointer to the deck to initialize.
 * @return 0 on success, -1 if memory allocation failed.
 */
int initDeck(Deck* deck) {
    deck->size = 52;
    deck->position = 0;

    deck->cards = (Card*)malloc(deck->size * sizeof(Card));
    if (deck->cards == NULL) {
        printf("Memory allocation failed in initDeck()\n");
        return -1;
    }

    int index = 0;
    for (int suit = 0; suit < 4; suit++) {
        for (int rank = 1; rank <= 13; rank++) {
            deck->cards[index].suit = suit;
            deck->cards[index].rank = rank;
            index++;
        }
    }

    shuffleDeck(deck);
    return 0;
}

/**
 * @brief Swaps two cards in place. Internal helper used by shuffleDeck().
 * @param a Pointer to the first card.
 * @param b Pointer to the second card.
 */
static void swap_cards(Card* a, Card* b) {
    Card temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Randomly shuffles the deck using the Fisher-Yates algorithm
 *        and resets the deal position back to the start.
 * @param deck Pointer to the deck to shuffle.
 */
void shuffleDeck(Deck* deck) {
    if (deck->cards == NULL || deck->size <= 1) {
        return;
    }

    // Fisher-Yates shuffle
    for (int i = deck->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap_cards(&deck->cards[i], &deck->cards[j]);
    }

    deck->position = 0;
}

/**
 * @brief Deals the next card from the deck.
 * @param deck Pointer to the deck to deal from.
 * @return The next Card in the deck, or an invalid card (suit = -1, rank = 0)
 *         if the deck is empty or uninitialized.
 */
Card dealCard(Deck* deck) {
    Card c;
    if (deck->cards == NULL || deck->position >= deck->size) {
        // indicate no card available
        c.suit = -1;
        c.rank = 0;
        return c;
    }

    c = deck->cards[deck->position];
    deck->position++;
    return c;
}

/**
 * @brief Resets the deck's deal position to the start and reshuffles it.
 * @param deck Pointer to the deck to reset.
 */
void resetDeck(Deck* deck) {
    if (deck->cards == NULL) {
        return;
    }
    deck->position = 0;
    shuffleDeck(deck);
}

/**
 * @brief Frees the memory allocated for the deck's card array.
 * @param deck Pointer to the deck to free.
 */
void freeDeck(Deck* deck) {
    if (deck->cards != NULL) {
        free(deck->cards);
        deck->cards = NULL;
    }
    deck->size = 0;
    deck->position = 0;
}