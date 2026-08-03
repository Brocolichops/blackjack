#include <stdio.h>
#include <stdlib.h>
#include "deck.h"

int initDeck(Deck* deck) {
    deck->cards = (Card*)malloc(DECK_SIZE * sizeof(Card));
    if (deck->cards == NULL) {
        return -1;
    }

    int index = 0;
    for (int suit = HEARTS; suit <= SPADES; suit++) {
        for (int rank = ACE; rank <= KING; rank++) {
            deck->cards[index].suit = (Suit)suit;
            deck->cards[index].rank = (Rank)rank;
            index++;
        }
    }

    deck->nextCardIndex = 0;
    return 0;
}

void shuffleDeck(Deck* deck) {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[j];
        deck->cards[j] = temp;
    }
}

Card dealCard(Deck* deck) {
    Card next = deck->cards[deck->nextCardIndex];
    deck->nextCardIndex++;
    return next;
}

void resetDeck(Deck* deck) {
    deck->nextCardIndex = 0;
    shuffleDeck(deck);
}

void freeDeck(Deck* deck) {
    free(deck->cards);
    deck->cards = NULL;
}