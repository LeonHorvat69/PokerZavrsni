#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "structs.h"
#include "functions.h"

#define HEART "\3"
#define DIAMOND "\4"
#define CLUB "\5"
#define SPADE "\6"

#define ANSI_COLOR_RESET   "\x1b[0m"
#define DIAMOND_COLOR_DARK_RED     "\x1b[31m"
#define CLUB_COLOR_DARK_WHITE     "\x1b[37m"
#define SPADE_COLOR_BRIGHT_WHITE     "\x1b[90m"
#define HEART_COLOR_BRIGHT_RED     "\x1b[91m"

#define checkFinalScore(previousScore) linearSearch(scoreListP, 10, previousScore)

static const int cardAmount = 52;
static const char(*handName[20]) = { "Royal Flush", "Four of a Kind", "Full House",
									 "Flush",       "Straight",       "Three of a Kind",
									 "Two Pair",    "One Pair",	      "High Card",	      "Empty hand" };

static const int handTypeChipsValues[10] = { 100, 60, 40,
											 35, 30, 30,
											 20, 10, 5, 0 };

static const int handTypeMultValues[10] = { 8, 7, 4,
								   		    4, 4, 3,
										    2, 2, 1, 0 };

extern int deckIndex;
static int scoreListP[10] = { 0 };
static int* scoreList = scoreListP;

// ASSIGN CARDS AND SPACE
CARD* dynamicSpace() {
	CARD* card = (CARD*)malloc(cardAmount * sizeof(CARD));
	if (card == NULL) {
		perror("Dynamic space allocation of CARDs failed.");
		exit(EXIT_FAILURE);
	}
	return card;
}
void assignRank(CARD* card) {
	//Give rank to all 52 cards
	int ranks = 1;
	for (int i = 0; i < cardAmount; i++) {
		(card + i)->rank = ranks;
		if (ranks == 13) {
			ranks = 0;
		}
		ranks++;
	}
}
void assignSuit(CARD* card) {
	//Give one of four suits to all cards
	const char(*a[4]) = { HEART, DIAMOND, CLUB, SPADE };
	int j = 0;
	for (int i = 0; i < cardAmount; i++) {
		strcpy((card + i)->suit, a[j]);
		if ((i + 1) % 13 == 0) {
			j++;
		}
	}
}
void shuffleDeck(CARD* deck[], CARD* allCardsInOrder[]) {
	srand((unsigned)time(NULL));
	int randCard = 0;
	int sortedNumbers[52] = { 0 };
	int cardsLeft = 52;
	for (int i = 0; i < 52; i++)
		sortedNumbers[i] = i;

	for (int i = 0; i < 52; i++) {
		int randNum = rand() % cardsLeft;
		randCard = sortedNumbers[randNum];
		deck[i] = allCardsInOrder[randCard];
		arraySwitchPlaceInt(&sortedNumbers[randNum], &sortedNumbers[cardsLeft - 1]);
		cardsLeft--;
	}
}

// PRINT HAND
void printGame(const PLAYER_INFO* playerInfo, const int discardUsed) {
	if (playerInfo->statistics->round != 1 && discardUsed == 0) {
		printf("\n\t  [Press any key to continue]\n");
		_getch();
	}

	system("cls");
	printInstructions();
	printf("Score  %d\n", playerInfo->score);
	printf("Discards  %d", playerInfo->discards);
	printf("\t\t\t\t  %d.  Round\n", playerInfo->statistics->round);
	for (int i = 0; i < 8; i++) {
		if (strcmp(playerInfo->hand[i]->suit, HEART) == 0) {
			printCardColor(playerInfo->hand[i], 0);
		}
		else if (strcmp(playerInfo->hand[i]->suit, DIAMOND) == 0) {
			printCardColor(playerInfo->hand[i], 1);
		}
		else if (strcmp(playerInfo->hand[i]->suit, CLUB) == 0) {
			printCardColor(playerInfo->hand[i], 2);
		}
		else if (strcmp(playerInfo->hand[i]->suit, SPADE) == 0) {
			printCardColor(playerInfo->hand[i], 3);
		}
	}

	putchar('\n');
	for (int i = 0; i < 8; i++) {
		printf("%d.     ", i + 1);
	}
	printf("  %d. end round\n", 0);
}
void printInstructions() {
	printf("Poker hands\t\t\t\n");
	printf("Royal Flush\t   100 X 8\t\t\t\n");
	printf("Four of a Kind\t   60 X 7\t\t\t\n");
	printf("Full House\t   40 X 4\t\t\t\n");
	printf("Flush\t\t   35 X 4\t\t\t\n");
	printf("Straight\t   30 X 4\t\t\t\n");
	printf("Three of a Kind\t   30 X 3\t\t\t\n");
	printf("Two pair\t   20 X 2\t\t\t\n");
	printf("Pair\t\t   10 X 2\t\t\t\n");
	printf("High Card\t   5 X 1\t\t\t\n");
	printf("\n\n");
}
void findCardSuit(PLAYER_INFO* playerInfo) {
	sortPlayedHand(playerInfo);

	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (strcmp(playerInfo->playedHand[i]->suit, HEART) == 0) {
			printCardColor(playerInfo->playedHand[i], 0);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, DIAMOND) == 0) {
			printCardColor(playerInfo->playedHand[i], 1);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, CLUB) == 0) {
			printCardColor(playerInfo->playedHand[i], 2);
		}
		else if (strcmp(playerInfo->playedHand[i]->suit, SPADE) == 0) {
			printCardColor(playerInfo->playedHand[i], 3);
		}
	}
}
void printCardColor(const CARD* hand, const int ChoseColor) {
	char(*collor[15]) = { HEART_COLOR_BRIGHT_RED, DIAMOND_COLOR_DARK_RED, CLUB_COLOR_DARK_WHITE, SPADE_COLOR_BRIGHT_WHITE };

	if (hand->rank < 10 && hand->rank != 1) {
		printf("%s%d%s     ", collor[ChoseColor], hand->rank, hand->suit);
	}
	else if (hand->rank == 10) {
		printf("%s%d%s    ", collor[ChoseColor], hand->rank, hand->suit);
	}
	else if (hand->rank == 1) {
		printf("%s%c%s     ", collor[ChoseColor], 'A', hand->suit);
	}
	else if (hand->rank == 11) {
		printf("%s%c%s     ", collor[ChoseColor], 'J', hand->suit);
	}
	else if (hand->rank == 12) {
		printf("%s%c%s     ", collor[ChoseColor], 'Q', hand->suit);
	}
	else if (hand->rank == 13) {
		printf("%s%c%s     ", collor[ChoseColor], 'K', hand->suit);
	}
	printf(ANSI_COLOR_RESET);
	return;

}

// PLAYING HAND
void playHand(PLAYER_INFO* playerInfo) {
	int numOfCardsPlayed = 0;
	int input = 0;
	do {
		input = getInput("gameInput");
		if (input == 0)
			break;

		if (input < 0) { //used for discarding cards
			if (playerInfo->discards == 0) {
				printf("\tUsed all discards!!!\n\n");
				continue;
			}
			playerInfo->hand[(input * -1) - 1] = playerInfo->deck[deckIndex];
			deckIndex++;
			playerInfo->discards--;
			printGame(playerInfo, 1);
			continue;

		}

		for (int i = 0; i < numOfCardsPlayed; i++) {
			if (playerInfo->playedHand[i]->rank == playerInfo->hand[input - 1]->rank &&
				playerInfo->playedHand[i]->suit == playerInfo->hand[input - 1]->suit) {
				input = getInput("gameInput"); //If card used repeat input
				if (input == 0)
					break;
			}
		}

		playerInfo->playedHand[numOfCardsPlayed] = playerInfo->hand[input - 1];
		numOfCardsPlayed++;
		playerInfo->statistics->allCardsPlayedInRound++;
	} while (numOfCardsPlayed < 5);
	
	playerInfo->cardsPlayed = numOfCardsPlayed;
	findCardSuit(playerInfo);
}
int getInput(char* chooseInput) {
	int min = 0;
	int max = 0;
	char* text = NULL;
	if (strcmp(chooseInput, "gameInput") == 0) {
		text = "Input cards to play: ";
		min = -8;
		max = 8;
	}
	else if (strcmp(chooseInput, "menuInput") == 0) {
		text = "Input menu option: ";
		min = 1;
		max = 6;
	}
	int input = 0;
	char keyInput[50];
	do {
		printf("%s", text);
		if (fgets(keyInput, 40, stdin) == NULL) {
			
			memset(keyInput, '\0', 40);
			input = getInput(chooseInput);
		}
		if (keyInput[0] == '0' && min != 1) {
			input = 0;
			return input;
		}
		input = atoi(keyInput);
		if (input == 0) {
			
			memset(keyInput, '\0', 40);
			input = getInput(chooseInput);
		}
		printf("\n");
		
	} while (input < min || input > max);
	return input;
}
void sortPlayedHand(PLAYER_INFO* playerInfo) {
	int arraySorted = 0;
	int count = 0;
	while (arraySorted == 0) {
		for (int i = 0; i < playerInfo->cardsPlayed - 1; i++) {
			if (playerInfo->playedHand[i]->rank > playerInfo->playedHand[i + 1]->rank) {
				arraySwitchPlace(playerInfo->playedHand, i, i + 1);
				count++;
			}
		}
		if (count == 0)
			arraySorted = 1;

		count = 0;
	}
}
inline void arraySwitchPlace(CARD* array[], const int indexA, const int indexB) {
	CARD* temp = array[indexA];
	array[indexA] = array[indexB];
	array[indexB] = temp;
}
inline void arraySwitchPlaceInt(int* biggerNum, int* smallerNum) {
	int temp = 0;
	temp = *smallerNum;
	*smallerNum = *biggerNum;
	*biggerNum = temp;
}

// SCORING
int scoreHand(PLAYER_INFO* playerInfo) {
	static int score = 0;
	int cardChips = 0;  //Chips earned from cards rank
	int handTypeChips = calculateChips(playerInfo, &cardChips); //Chips earned from hand type
	int mult = calculateMult(playerInfo);
	printf("\n\n");
	printf("\tChips  X  Multiplier   Chip Bonus\n");
	printf("          %d     ", handTypeChips);
	printf("      %d ", mult);
	printf("\t  +%d \n", cardChips);
	printf("\n\n");
	printf("\t          Score: %d ", mult * handTypeChips + cardChips);
	score += mult * handTypeChips + cardChips;
	playerInfo->score = score;
	if (playerInfo->statistics->round == 3) 
		score = 0;
	return playerInfo->score;
}
int calculateChips(const PLAYER_INFO* playerInfo, int* cardChips) {
	long int handTypeChips = 0;
	char* handValue = handType(playerInfo);
	if (handValue == NULL) {
		printf("error");
		exit(-1);
	}
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (playerInfo->playedHand[i]->rank == 1) {  // Aces give 14 chips while other cards give their value
			*cardChips += 14;
			continue;
		}
		*cardChips += playerInfo->playedHand[i]->rank;
	}

	for (int i = 0; i < 10; i++) {
		if (strcmp(handValue, handName[i]) == 0) {
			handTypeChips = handTypeChipsValues[i];
			break;
		}
	}
	return handTypeChips;
}
int calculateMult(const PLAYER_INFO* playerInfo) {
	char* handValue = handType(playerInfo);
	puts(handValue); //Shows played hand type
	for (int i = 0; i < 10; i++) {
		if (strcmp(handValue, handName[i]) == 0) {
			return handTypeMultValues[i];
		}
	}
	printf("\nError - hand name not found");
	exit(-1);
}
char* handType(const PLAYER_INFO* playerInfo) {
	int tempArray[5] = { 0 };
	int rankNum = arrayGetSingleNumber(playerInfo, sameRankCount(playerInfo, tempArray));
	if (playerInfo->cardsPlayed == 0) {
		return "Empty hand";
	}
	if (playerInfo->cardsPlayed == 1) {
		return "High Card";
	}
	if (playerInfo->cardsPlayed == 5) {
		if ((strcmp(playerInfo->playedHand[0]->suit, playerInfo->playedHand[1]->suit) == 0) && (strcmp(playerInfo->playedHand[1]->suit, playerInfo->playedHand[2]->suit) == 0) &&
			(strcmp(playerInfo->playedHand[2]->suit, playerInfo->playedHand[3]->suit) == 0) && (strcmp(playerInfo->playedHand[3]->suit, playerInfo->playedHand[4]->suit) == 0)) {

			if (playerInfo->playedHand[1]->rank == (playerInfo->playedHand[0]->rank) + 1 && playerInfo->playedHand[2]->rank == (playerInfo->playedHand[0]->rank) + 2 &&
				playerInfo->playedHand[3]->rank == (playerInfo->playedHand[0]->rank) + 3 && playerInfo->playedHand[4]->rank == (playerInfo->playedHand[0]->rank) + 4)
				return "Royal Flush";

			else if (rankNum != 4 && rankNum != 32)
				return "Flush";
		}
	}

	if (rankNum == 4)
		return "Four of a Kind";

	if (rankNum == 32)
		return "Full House";

	if (playerInfo->cardsPlayed == 5)
		if (playerInfo->playedHand[1]->rank == playerInfo->playedHand[0]->rank + 1 && playerInfo->playedHand[2]->rank == playerInfo->playedHand[0]->rank + 2 &&
			playerInfo->playedHand[3]->rank == playerInfo->playedHand[0]->rank + 3 && playerInfo->playedHand[4]->rank == playerInfo->playedHand[0]->rank + 4)
			return "Straight";

	if (rankNum == 3)
		return "Three of a Kind";

	if (rankNum == 22)
		return "Two Pair";

	if (rankNum == 2)
		return "One Pair";

	return "High Card";
}
int* sameRankCount(const PLAYER_INFO* playerInfo, int* array) {
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		for (int j = 0; j < playerInfo->cardsPlayed; j++) {
			if ((playerInfo->playedHand[i]->rank == playerInfo->playedHand[j]->rank) && i != j) {
				array[i]++;
			}
		}
	}
	return array;
}
int arrayGetSingleNumber(const PLAYER_INFO* playerInfo, const int* sameRankList) {
	int arr[5] = { 0 };
	int numOfValues = 0;
	for (int i = 0; i < playerInfo->cardsPlayed; i++) {
		if (sameRankList[i] != 0) {
			numOfValues++;
			arr[i] = sameRankList[i];
		}
	}
	if (playerInfo->cardsPlayed == 5) { 
		if (playerInfo->playedHand[0]->rank == playerInfo->playedHand[1]->rank &&
			playerInfo->playedHand[4]->rank == playerInfo->playedHand[3]->rank && 
			(playerInfo->playedHand[2]->rank == playerInfo->playedHand[3]->rank || 
			playerInfo->playedHand[2]->rank == playerInfo->playedHand[1]->rank)) {
			return 32;  //Full house
		}
	}
	if (numOfValues == 4) {  
		if ((playerInfo->playedHand[1]->rank == playerInfo->playedHand[2]->rank &&
			playerInfo->playedHand[2]->rank == playerInfo->playedHand[3]->rank) &&
			(playerInfo->playedHand[0]->rank == playerInfo->playedHand[1]->rank ||
				playerInfo->playedHand[4]->rank == playerInfo->playedHand[3]->rank)) {
			return 4; //Four of a kind
		}
		else return 22;  //Two pair
	}
	if (numOfValues == 3) {
		return 3;  // Three of a kind
	}
	if (numOfValues == 2) {
		return 2; //Pair
	}
	return 0;
}
void printFinalScreen(const PLAYER_INFO* playerInfo) {
	printf("\n\t  [Press any key to continue]\n");
	_getch();
	system("cls");
	char spacing[] = "   ";  //Used to allign final score to screen

	if (playerInfo->score < 100 && playerInfo->score > 9) {
		strcpy(spacing, "  ");
	}
	else if (playerInfo->score > 99 ) {
		strcpy(spacing, " ");
	}
	printf("\n\n\n\n");
	printf("                     --------------------------------       \n");
	printf("                    |            Final score         |      \n");
	printf("                    |                %d     %s       |      \n", playerInfo->score, spacing);
	printf("                     --------------------------------       \n");
	printScoreList();
	int previousScore = checkFinalScore(playerInfo->score);
	if (previousScore == -1) {
		printf("\n\n\t\tFinal score did not make it to the leaderboard\n");
	}
	else {
		printf("\n\n\t\tFinal score took the %d. place on the leaderboard\n", 10 - previousScore);
	}
	Sleep(6500);
}
void saveScore(const int score) {
	char temp[30] = { '\0' };
	FILE* fp = NULL;
	if (fopen("highScore.txt", "r") != NULL) {
		fp = fopen("highScore.txt", "r");
		fgets(temp, 10, fp);
		int temp2 = atoi(temp);
		if (temp2 > score) {
			return;
		}
	}
	fp = fopen("highScore.txt", "w");
	if (fp == NULL) {
		perror("File pointer empty ");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%d", score);
	fclose(fp);
}

//MENU
int printMenu(char* currentUser) {
	system("cls");
	printf("Current user: %s", currentUser);
	printf("1. Start game\n");
	printf("2. Tutorial\n");
	printf("3. High score\n");
	printf("4. Exit game\n");
	printf("5. New user\n");
	printf("6. Load different user\n");
	int input = getInput("menuInput");
	system("cls");
	return input;
}




void printGameInstructions() {
	printf("Each round you are handed 8 cards and from them choose 5 cards or less to play.\n");
	printf("You can discard up to 3 cards in a round to create a new hand.\n");
	printf("All hand types have different values and multipliers which add up with the worth of each rank from each card.\n");
	printf("The goal is to get as high of a score as you can.\n");
	printf("\n\t\t  [Press any key to continue]\n");
	_getch();
}
void showHighScore() {
	int highScore = 0;
	FILE* fp = fopen("highScore.txt", "r");
	if (fp == NULL) {
		printf("No high score");
		Sleep(5000);
		return;
	}
	if (fscanf(fp, "%d", &highScore) == NULL) {
		perror("ERROR: ");
		exit(-1);
	}
	printf("\n\n\n\t\t      High score is %d", highScore);
	printf("\n\t\t  Press any key to continue\n\n");
	_getch();
	fclose(fp);
}
void openSubMenu(int choice) {
	void (*functionP[2])();
	functionP[0] = showHighScore;
	functionP[1] = printGameInstructions;
	if (choice == 3) {
		functionP[0]();
		return;
	}
	if (choice == 2) {
		functionP[1]();
	}
}

//SCOREBOARD
void addScoreToList(const int score) {
	FILE* fp = fopen("scoreList.txt", "r");
	if (fp == NULL) {
		createList(fp, score);
		return;
	}
	if (fp == NULL) {
		perror("Cant create scoreList");
		exit(EXIT_FAILURE);
	}
	getSavedList(fp);
	fp = fopen("scoreList.txt", "w");
	insertScore(score);
	updateList(fp);
	fclose(fp);
}
void createList(FILE* fp, const int score) {
	fp = fopen("scoreList.txt", "w");
	if (fp == NULL) {
		perror("Cant create scoreList.txt");
		exit(EXIT_FAILURE);
	}
	rewind(fp);
	fprintf(fp, "%d ", score);
	*(scoreList + 0) = score;
	for (int i = 1; i < 10; i++) 
		fprintf(fp, "0 ");
	fclose(fp);
}
inline void getSavedList(FILE* fp) {
	for (int i = 0; i < 10; i++) {
		if (fscanf(fp, "%d", &scoreListP[i]) == EOF) {
			perror("Cant get saved list");
			exit(EXIT_FAILURE);
		}
	}
	fclose(fp);
}
void insertScore(const int score) {
	if (*(scoreList + 0) > score)
		return;
	*(scoreList + 0) = score;
	selectionSort(scoreList, 10);
}
void sortScores() {
	int arraySorted = 0;
	int count = 0;
	while (arraySorted == 0) {
		for (int i = 0; i < 9; i++) {
			if (*(scoreList + i) < *(scoreList + (i + 1))) {
				arraySwitchPlaceInt(&scoreList[i], &scoreList[i + 1]);
				count++;
			}
		}
		if (count == 0)
			arraySorted = 1;
		count = 0;
	}
}
void updateList() {
	FILE* fp = fopen("scoreList.txt", "w");
	if (fp == NULL) {
		perror("Cant create scoreList");
		exit(EXIT_FAILURE);
	}
	rewind(fp);
	for (int i = 0; i < 10; i++) {
		fprintf(fp, "%d ", *(scoreList + i));
	}
	fclose(fp);
}

void newUser() {
	char name[30] = { '\0' };
	printf("Create new username: ");
	fgets(name, 30, stdin);
	FILE* fp = fopen("usernames.txt", "w");
	if (fp == NULL) {
		perror("No data");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%s\n", name);

	fclose(fp);

}
char* loadUser(char* username) {
	char user[30] = { "\0" };
	
}


void selectionSort(int array[], const int n) {
	int min = -1;
	for (int i = 0; i < n - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (array[j] < array[min]) {
				min = j;
			}
		}
		arraySwitchPlaceInt(&array[i], &array[min]);
	}
}
void printScoreList() {
	printf("\n\t\tSCOREBOARD");
	for (int i = 0; i < 9; i++) {
		printf("\n\t\t    %d.  %d ", i + 1, scoreListP[9 -  i]);
	}
	printf("\n\t\t   %d.  %d ", 10, scoreListP[0]);
}
int linearSearch(const int array[], const int n, const int num) {
	for (int i = 0; i < n; i++)
	{
		if (array[i] == num) {
			return i;
		}
	}
	return -1;
}

