#ifndef DECK_H
#define DECK_H

#define DECK_SIZE 52

typedef enum {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
} Suit;

typedef enum {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
    EIGHT, NINE, TEN, JACK, QUEEN, KING
} Rank;

typedef struct {
    Suit suit;
    Rank rank;
} Card;

typedef struct {
    Card* cards;        
    int nextCardIndex;  
} Deck;

int initDeck(Deck* deck);
void shuffleDeck(Deck* deck);
Card dealCard(Deck* deck);
void resetDeck(Deck* deck);
void freeDeck(Deck* deck);

#endif