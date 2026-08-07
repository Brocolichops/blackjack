#pragma once
#ifndef DECK_H
#define DECK_H

/** A single playing card. */
typedef struct {
    int suit;  /**< 0=Hearts, 1=Diamonds, 2=Clubs, 3=Spades */
    int rank;  /**< 1=Ace, 2-10, 11=Jack, 12=Queen, 13=King */
} Card;

/** A deck of playing cards. */
typedef struct {
    Card* cards;   /**< array of cards */
    int size;      /**< total cards (should be 52) */
    int position;  /**< index of the next card to deal */
} Deck;

/** Builds and shuffles a new 52-card deck. Returns 0 on success, -1 on failure. */
int initDeck(Deck* deck);

/** Shuffles the deck. */
void shuffleDeck(Deck* deck);

/** Deals the next card from the deck. */
Card dealCard(Deck* deck);

/** Resets the deck to the top and reshuffles it. */
void resetDeck(Deck* deck);

/** Frees the deck's memory. */
void freeDeck(Deck* deck);

#endif