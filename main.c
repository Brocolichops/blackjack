#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "deck.h"

static Deck gameDeck;

int initializeGame(void) {
    srand((unsigned int)time(NULL));

    if (initDeck(&gameDeck) != 0) {
        return -1;
    }
    shuffleDeck(&gameDeck);

    return 0;
}

void cleanupGame(void) {
    freeDeck(&gameDeck);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        printf("Running with argument: %s\n", argv[1]);
    }

    if (initializeGame() != 0) {
        fprintf(stderr, "Failed to initialize game.\n");
        return 1;
    }

    cleanupGame();
    return 0;
}