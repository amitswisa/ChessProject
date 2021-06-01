#ifndef _KNIGHT_CLASS
#define _KNIGHT_CLASS

#include "head.h"

//Question 1
chessPosArray*** validKnightMoves();
chessPosArray* knightMovesPossibilities(char rowPos, char colPos);

//Question 3
pathTree findAllPossibleKnightPaths(chessPos* startingPosition);
treeNode* findAllPossibleKnightPathsHelper(bool knightMoveRecord[][BOARD_SIZE], chessPos* pos, chessPosArray*** knightMoves);
treeNode* createNewTreeNode(chessPos* position, treeNodeListCell* posListHead);

// Question 4
chessPosList* findKnightPathCoveringAllBoard(pathTree* path_tree);
chessPosCell* findKnightPathCoveringAllBoardHelper(treeNode* root, int treeLevel, chessPosList* posList);
chessPosCell* createNewListNode(chessPos* cellPos, chessPosCell* next);

#endif