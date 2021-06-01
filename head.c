#include "head.h"

void display(chessPosList* lst)
{
	int board[BOARD_SIZE][BOARD_SIZE] = { 0 };

	// insert id of node (position of node in list) into board array by node chessPos position.
	updateBoardWithPositions(board, lst);

	// Display board with updated values.
	printBoard(board);
}

void printBoard(int board[][BOARD_SIZE])
{
	for (int i = 0;i <= BOARD_SIZE;i++) {
		printf("+_____");
	}
	printf("+\n");
	for (int i = 0; i <= BOARD_SIZE; i++)
	{
		if (i == 0)
		{
			printf("|  %3c|", ' ');
		}
		else
		{
			printf("| %3c |", (i - 1) + 'A');
		}

		for (int j = 1; j <= BOARD_SIZE; j++)
		{
			if (i == 0)
				printf(" %3d |", j);
			else
			{
				if (board[i - 1][j - 1] == 0)
				{
					printf("  %3c|", ' ');
				}
				else {
					printf(" %3d |", board[i - 1][j - 1]);
				}
			}

		}
		printf("\n");
		for (int i = 0;i <= BOARD_SIZE;i++) {
			printf("+_____");
		}
		printf("+\n");
	}
}

void updateBoardWithPositions(int board[][BOARD_SIZE], chessPosList* lst) {

	/*
		# write position of node in list into board array.
	*/

	chessPosCell* curr = lst->head;
	chessPosCell* prev = NULL;
	int counter = 1;

	while (curr != NULL) {

		int x = curr->position[0] - 'A';
		int y = curr->position[1] - '1';

		if (board[x][y] != 0)
		{
			// Delete node that already exists from list.
			if (lst->tail == curr)
				lst->tail = prev;

			prev->next = curr->next;
			free(curr);
		}
		else {
			board[x][y] = counter;
			counter++;
			prev = curr;
		}

		curr = curr->next;
	}

}

char* getFileNameFromUser() {

	printf("Please enter path of file (Example: file.bin): ");

	int phySize = 1, logSize = 0;
	char* fileName = (char*)malloc(sizeof(char) * phySize);
	checkMemoryAllocation(fileName);

	char buffer = NULL;
	scanf("%c", &buffer);

	bool keep = true;
	while (buffer != '\n' && buffer != NULL) {

		fileName[logSize] = buffer;
		logSize++;

		if (logSize == phySize)
		{
			phySize *= 2;
			fileName = (char*)realloc(fileName, sizeof(char) * phySize);
		}

		buffer = getchar();
	}

	fileName = (char*)realloc(fileName, sizeof(char) * logSize + 1);
	fileName[logSize] = '\0';

	return fileName;
}

bool isInsideBoardBoundries(char xCoordKnightMove, char yCoordKnightMove) {

	return ((xCoordKnightMove >= 'A' && xCoordKnightMove < BOARD_SIZE + 'A')
		&& (yCoordKnightMove >= '1' && yCoordKnightMove < BOARD_SIZE + '1'));

}

void checkMemoryAllocation(void* p) {

	/* Check if memory allocation failed  */

	if (p == NULL)
	{
		printf("Memory allocation failed!!");
		exit(1);
	}

}

void freePathTree(pathTree* pt)
{
	if (pt == NULL || pt->root == NULL)
		return;

	freePathTreeHelper(pt->root);
	pt->root = NULL;
}

void freePathTreeHelper(treeNode* node)
{
	if (node == NULL)
		return;

	treeNodeListCell* lst = node->next_possible_positions;
	treeNodeListCell* next = NULL;
	while (lst != NULL)
	{
		freePathTreeHelper(lst->node);
		next = lst->next;
		free(lst);
		lst = next;
	}

	free(node);
}

void freeValidKnightMoves(chessPosArray*** knightMoves) {

	for (int i = 0;i < BOARD_SIZE;i++) {

		for (int j = 0;j < BOARD_SIZE;j++) {

			free(knightMoves[i][j]->positions);
			free(knightMoves[i][j]);

		}

		free(knightMoves[i]);

	}

	free(knightMoves);

}

void freeList(chessPosList* lst)
{
	if (lst->head == NULL)
		return;

	chessPosCell* curr = lst->head;
	while (curr != NULL) {
		chessPosCell* temp = curr->next;
		free(curr);
		curr = temp;
	}

	lst->head = NULL;

}

void freeAll(pathTree* pt, chessPosList* lst) {

	freePathTree(pt);
	freeList(lst);
}

void clearBuffer() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}