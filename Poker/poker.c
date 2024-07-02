#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

#define MAX_HAND_SIZE 8
#define FIRST_ROUND  1
#define LAST_ROUND  3

int deckIndex = 0;
int main() {
	CARD* card = NULL;
	CARD* allCardsInOrder[52] = { NULL };
	CARD* deck[52] = { NULL };

	card = dynamicSpace();
	assignRank(card);
	assignSuit(card);

	PLAYER_INFO playerInfo;
	PLAYER_INFO* playerInfoP = &playerInfo;
	STATISTICS statistics;
	STATISTICS* statisticsP = &statistics;
	if (playerInfoP == NULL || statisticsP == NULL) {
		perror("Empty pointers");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 52; i++) {
		allCardsInOrder[i] = (card + i);
		playerInfo.deck[i] = (card + i);
		if (allCardsInOrder[i] == NULL || playerInfo.deck[i] == NULL) {
			perror("Empty pointers");
			exit(EXIT_FAILURE);
		}
	}
	
	playerInfo.cardsPlayed = 0;
	playerInfo.score = 0;
	playerInfo.statistics = statisticsP;
	statistics.round = 1;
	shuffleDeck(playerInfoP->deck, allCardsInOrder);

	FILE* fp = fopen("scoreList.txt", "r");
	getSavedList(fp);
	USERS users[10];
	USERS* usersP = users;
	int currentUser = 0;
	if (fopen("users.bin", "rb") == NULL) {
		for (int i = 0; i < 10; i++) {
		strcpy(users[i].name, "Empty user");
		for (int j = 0; j < 10; j++) {
			users[i].scores[j] = 0;
		}
	}
		char name[30] = {'\0'};
	printf("Enter username: ");
	int f = scanf("%s", name);
	char* g = strcpy(users[0].name, name);
	saveUsers(users);
	}
	else {
		usersP = loadUser(users);
		currentUser = selectUser(users);
	}

	int choice = 0;
	while (1) {   
		choice = printMenu((usersP + currentUser)->name);
		openSubMenu(choice);

		while (choice == START_GAME) { 
			playerInfo.discards = 3;
			for (int i = 0; i < MAX_HAND_SIZE; i++) {
				playerInfoP->hand[i] = playerInfoP->deck[deckIndex];
				deckIndex++;
			}
			if (deckIndex > 40) {
				deckIndex = 0;
				shuffleDeck(playerInfoP->deck, allCardsInOrder);
			}
			printGame(playerInfoP, 0);
			playHand(playerInfoP);
			playerInfo.score = scoreHand(playerInfoP);
			if (statistics.round == LAST_ROUND) { 


				statistics.round = FIRST_ROUND;
				addScoreToList(playerInfo.score);
				saveScore(playerInfo.score);
				saveScoreForUser(users, currentUser, playerInfo.score);
				printFinalScreen(playerInfoP);
				playerInfo.score = 0;
				break;
			}
			statistics.round++;
		}

		if (choice == NEW_PLAYER) 
			newUser(users);
			
		if (choice == LOAD_PLAYER) 
			currentUser = selectUser(users);
			
		if (choice == DELETE_PLAYER) 
			usersP = deletedUser(users);
			
		if (choice == USER_HIGH_SCORES)
			printUserScores(currentUser);

		if (choice == EXIT) {
			free(card);
			card = NULL;
			playerInfoP = NULL;
			statisticsP = NULL;
			for (int i = 0; i < 52; i++) {
				allCardsInOrder[i] = NULL;
				deck[i] = NULL;
			}
			return 0;
		}
	}
}

