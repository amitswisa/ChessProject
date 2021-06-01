#include "knightClass.h"

//Question 1
chessPosArray*** validKnightMoves() {

	/*
		# this function finds and create an array that stores knight possisble movement from each position on board.
		# return: amount of possible liegal movements and their locations on board.
	*/

	chessPosArray*** boardPointer = (chessPosArray***)malloc(sizeof(chessPosArray**) * BOARD_SIZE);
	checkMemoryAllocation(boardPointer);

	for (int c = 'A'; c < BOARD_SIZE + 'A'; c++)
	{
		boardPointer[c - 'A'] = (chessPosArray**)malloc(BOARD_SIZE * sizeof(chessPosArray*));
		checkMemoryAllocation(boardPointer[c - 'A']);

		for (int n = '1'; n < BOARD_SIZE + '1'; n++)
		{
			boardPointer[c - 'A'][n - '1'] = knightMovesPossibilities(c, n);
		}
	}

	return boardPointer;

}

chessPosArray* knightMovesPossibilities(char rowPos, char colPos) {

	/*
		chessPosArray knightMovesPossibilities

		# this function finds every possible movement of a knight from specific position on board.
		# returned values: amount of possible positions, which positions from current position on board are valid.
	*/
	chessPosArray* boardSqaure = (chessPosArray*)malloc(sizeof(chessPosArray));
	checkMemoryAllocation(boardSqaure);

	boardSqaure->size = 0;

	boardSqaure->positions = (chessPos*)malloc(KNIGHT_MOVES_SIZE * sizeof(chessPos));
	checkMemoryAllocation(boardSqaure->positions);

	// All possible moves of a knight
	int horizontal[KNIGHT_MOVES_SIZE] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int vertical[KNIGHT_MOVES_SIZE] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	for (int i = 0; i < KNIGHT_MOVES_SIZE; i++)
	{
		// find location on board for each knight's move from specified position.
		char xCoordKnightMove = rowPos + horizontal[i];
		char yCoordKnightMove = colPos + vertical[i];

		// check if move is valid - knight in board's boundaries.
		if (isInsideBoardBoundries(xCoordKnightMove, yCoordKnightMove))
		{
			boardSqaure->positions[boardSqaure->size][0] = xCoordKnightMove;
			boardSqaure->positions[boardSqaure->size][1] = yCoordKnightMove;
			boardSqaure->size++;
		}
	}

	// Reallocate memory to logic size.
	boardSqaure->positions = (chessPos*)realloc(boardSqaure->positions, boardSqaure->size * sizeof(chessPos));
	checkMemoryAllocation(boardSqaure->positions);

	return boardSqaure;
}

//Question 3

pathTree findAllPossibleKnightPaths(chessPos* startingPosition) {

	pathTree tr;
	chessPosArray*** knightMoves = validKnightMoves();
	bool knightMoveRecord[BOARD_SIZE][BOARD_SIZE] = { false };

	tr.root = findAllPossibleKnightPathsHelper(knightMoveRecord, startingPosition, knightMoves);

	freeValidKnightMoves(knightMoves);

	return tr;
}

treeNode* findAllPossibleKnightPathsHelper(bool knightMoveRecord[][BOARD_SIZE], chessPos* pos, chessPosArray*** knightMoves) {

	/*
		# Function find all routes of a knight from specific position on board.
	*/

	int row = pos[0][0] - 'A';
	int col = pos[0][1] - '1';

	// create move node.
	treeNode* moveNode = createNewTreeNode(pos, NULL);

	// Find all possible moves from current location.
	chessPosArray* nextMovesArray = knightMoves[row][col];
	if (nextMovesArray->size == 0)
		return moveNode;

	// Set as visited cell is board.
	knightMoveRecord[row][col] = true;

	treeNodeListCell* lastList = NULL;

	for (int i = 0; i < nextMovesArray->size; i++) {

		char moveRow = nextMovesArray->positions[i][0];
		char moveCol = nextMovesArray->positions[i][1];
		// If knight has already been in current location. || PROBLEM!!!
		if (knightMoveRecord[moveRow - 'A'][moveCol - '1'] == false)
		{
			treeNodeListCell* listNode = (treeNodeListCell*)malloc(sizeof(treeNodeListCell));
			checkMemoryAllocation(listNode);

			chessPos tempPos = { moveRow, moveCol };

			listNode->next = NULL;
			listNode->node = findAllPossibleKnightPathsHelper(knightMoveRecord, &tempPos, knightMoves);

			if (moveNode->next_possible_positions == NULL)
			{
				moveNode->next_possible_positions = listNode;
				lastList = listNode;
			}
			else {
				lastList->next = listNode;
				lastList = listNode;
			}
		}
	}

	knightMoveRecord[row][col] = false;
	return moveNode;
}

treeNode* createNewTreeNode(chessPos* position, treeNodeListCell* posListHead) {

	treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
	checkMemoryAllocation(newNode);

	newNode->position[0] = position[0][0];
	newNode->position[1] = position[0][1];
	newNode->next_possible_positions = posListHead;

	return newNode;
}

//Question 4


chessPosList* findKnightPathCoveringAllBoard(pathTree* path_tree)
{
	// Allocate memory for chessPosList.
	chessPosList* posList = (chessPosList*)malloc(sizeof(chessPosList));
	checkMemoryAllocation(posList);

	// make posList empty list.
	posList->head = posList->tail = NULL;

	int counterTreeLevel = 1; // counter of tree level.

	posList->head = findKnightPathCoveringAllBoardHelper(path_tree->root, counterTreeLevel, posList);

	return posList;
}

chessPosCell* findKnightPathCoveringAllBoardHelper(treeNode* root, int treeLevel, chessPosList* posList)
{

	if (root == NULL || treeLevel > BOARD_CELLS_NUM)
		return NULL;

	// if found last cell of path in tree.
	if (treeLevel == BOARD_CELLS_NUM)
	{
		posList->tail = createNewListNode(root->position, NULL);
		return posList->tail;
	}

	treeNodeListCell* currCell = root->next_possible_positions;

	while (currCell != NULL)
	{
		chessPosCell* newCell = findKnightPathCoveringAllBoardHelper(currCell->node, treeLevel + 1, posList);

		if (newCell != NULL)
		{
			return createNewListNode(root->position, newCell);
		}

		currCell = currCell->next;
	}

	return NULL;
}

chessPosCell* createNewListNode(chessPos* cellPos, chessPosCell* next)
{
	chessPosCell* newCell = (chessPosCell*)malloc(sizeof(chessPosCell));
	checkMemoryAllocation(newCell);

	newCell->position[0] = cellPos[0][0];
	newCell->position[1] = cellPos[0][1];
	newCell->next = next;

	return newCell;
}
