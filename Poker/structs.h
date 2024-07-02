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

typedef struct {
	char name[30];
	int scores[10];
}USERS;

typedef enum {
	START_GAME = 1,
	INSTRUCTIONS,
	HIGH_SCORE,
	NEW_PLAYER,
	LOAD_PLAYER,
	DELETE_PLAYER,
	USER_HIGH_SCORES,
	EXIT,
}MENU;




