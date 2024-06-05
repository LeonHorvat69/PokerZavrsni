#pragma once

// ASSIGN CARDS AND SPACE
CARD* dynamicSpace();
void assignRank(CARD* card);
void assignSuit(CARD* card);
void shuffleDeck(CARD* deck[], CARD* allCardsInOrder[]);

// PRINT HAND
void printGame(const PLAYER_INFO* playerInfo, const int discardUsed);
void printInstructions();
void findCardSuit(PLAYER_INFO* playerInfo);
void printCardColor(const CARD* hand, const int ChoseColor);

// PLAYING HAND
void playHand(PLAYER_INFO* playerInfo);
void sortPlayedHand(PLAYER_INFO* playerInfo);
inline void arraySwitchPlace(CARD* array[], const int indexA, const int indexB);
inline void arraySwitchPlaceInt(int* biggerNum, int* smallerNum);
int getInput(char*);

// SCORING
int scoreHand(PLAYER_INFO* playerInfo);
int calculateChips(const PLAYER_INFO* playerInfo, int* cardChips);
int calculateMult(const PLAYER_INFO* playerInfo);
char* handType(const PLAYER_INFO* playerInfo);
int* sameRankCount(const PLAYER_INFO* playerInfo, int* array);
int arrayGetSingleNumber(const PLAYER_INFO* playerInfo, const int* sameRankList);
void printFinalScreen(const PLAYER_INFO* playerInfo);
void saveScore(const int score);

//MENU
int printMenu();
void printGameInstructions();
void showHighScore();
void openSubMenu(int choice);

//LEADERBOARD
void addScoreToList(const int score);
void createList(FILE* fp, const int score);
inline void getSavedList(FILE* fp);
void insertScore(const int score);
void sortScores();
void updateList();
void selectionSort(int polje[], const int n);
inline void arraySwitchPlaceInt(int* biggerNum, int* smallerNum);
void printScoreList();
int linearSearch(const int array[], const int n, const int num);

