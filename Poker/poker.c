#include <stdio.h>
#include <stdlib.h>


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


	int choice = 0;
	while (1) {   // Menu
		choice = printMenu();
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
				saveScore(playerInfo.score);
				addScoreToList(playerInfo.score);
				printFinalScreen(playerInfoP);
				playerInfo.score = 0;
				break;
			}
			statistics.round++;
		}

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
		if (choice == 5) {
			newUser();
		}
		if (choice == 6) {

		}
	}
}

