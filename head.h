#ifndef _BOARD_HEADER
#define _BOARD_HEADER

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// General define
#define BOARD_SIZE 5
#define BOARD_CELLS_NUM BOARD_SIZE*BOARD_SIZE
#define KNIGHT_MOVES_SIZE 8

// Display path from file options
#define NOT_FOUND -1
#define NOT_VALID_PATH 1
#define COVER_BOARD 2
#define NO_COVER_BOARD 3
#define ELSE 0

// Colors define
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef unsigned char BYTE;

typedef char chessPos[2]; // define new struct, char's array chesPoss.

// define new struct [chessPosArray]
typedef struct _chessPosArray {
	unsigned int size;
	chessPos* positions;
} chessPosArray;

typedef struct _chessPosCell {
	chessPos position;
	struct _chessPosCell* next;
} chessPosCell;

typedef struct _chessPosList {
	chessPosCell* head;
	chessPosCell* tail;
} chessPosList;

// Question 3 structs
typedef struct _treeNodeListCell treeNodeListCell;

typedef struct _treeNode
{
	chessPos position;
	treeNodeListCell* next_possible_positions;
}treeNode;

typedef struct _treeNodeListCell
{
	treeNode* node;
	struct _treeNodeListCell* next;
} treeNodeListCell;

typedef struct _pathTree
{
	treeNode* root;
}pathTree;

// Question 2 functions
void display(chessPosList* lst);
void updateBoardWithPositions(int board[][BOARD_SIZE], chessPosList* lst);
void printBoard(int board[][BOARD_SIZE]);

bool isInsideBoardBoundries(char xCoordKnightMove, char yCoordKnightMove);
char* getFileNameFromUser();
void freePathTree(pathTree* pt);
void freePathTreeHelper(treeNode* node);
void checkMemoryAllocation(void* p);
void freeValidKnightMoves(chessPosArray*** knightMoves);
void freeList(chessPosList* lst);
void freeAll(pathTree* pt, chessPosList* lst);
void clearBuffer();
#endif