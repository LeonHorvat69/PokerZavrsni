#pragma once

typedef struct {
	int rank;
	char suit[2];
}CARD;

typedef struct {
	int allCardsPlayedInRound;
	int round;
	int previousScore;
}STATISTICS;

typedef struct {
	CARD* hand[8];
	CARD* playedHand[5];
	CARD* deck[52];
	int score;
	int discards;
	int cardsPlayed;
	STATISTICS* statistics;
}PLAYER_INFO;


