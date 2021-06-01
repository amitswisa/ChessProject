#include "fileOps.h"

//Question 5

short int numOfNodesInList(chessPosCell* node)
{
	/*
		Count and return number of nodes in list.
	*/
	chessPosCell* currNode = node;
	short int count = 0;

	while (currNode != NULL)
	{
		count++;
		currNode = currNode->next;
	}

	return count;
}

int getSizeOfBytesInFile(short int numOfNodes)
{
	// Calculate size of BYTE array.
	numOfNodes *= 6; // Num of bits required.

	if (numOfNodes % 8 == 0)
		return numOfNodes / 8;

	numOfNodes += (8 - (numOfNodes % 8)); // add the zero "left over" bits.
	numOfNodes /= 8;

	return numOfNodes;
}

void saveListToBinFile(char* file_name, chessPosList* pos_list) {

	// Find amount of nodes in list.
	short int numOfNodes = numOfNodesInList(pos_list->head);

	if (numOfNodes <= 0)
	{
		printf("\n%s*Error: list is empty (no path).%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		return;
	}

	FILE* locationFile = fopen(file_name, "wb");
	checkMemoryAllocation(locationFile);

	// Write num of nodes to file.
	fwrite(&numOfNodes, sizeof(short int), 1, locationFile);

	chessPosCell* curr = pos_list->head;

	// Calculate size of BYTE array.
	numOfNodes = getSizeOfBytesInFile(numOfNodes);

	BYTE* arrOfByte = (BYTE*)malloc((numOfNodes) * sizeof(BYTE));
	checkMemoryAllocation(arrOfByte);

	BYTE mask[3] = { 0xC0, 0xF0 , 0xFC }; // {{MASK 1: 11000000} {MASK 2: 11110000} {MASK 3: 11111100}}

	int index = 0;
	BYTE temp = 0;

	int fixIndexJumps = 0;

	while (curr != NULL)
	{
		// Represent each pos with 3 bits from left to right.
		// Example: [C,5] -> C-010 5-100 EXTRA-00 -> 01010000
		BYTE rowPos = ((BYTE)curr->position[0] - 'A') << 5;
		BYTE colPos = ((BYTE)curr->position[1] - '1') << 2;
		BYTE res = rowPos | colPos;

		if (index % 4 == 0)
		{
			temp = res;
			arrOfByte[index - fixIndexJumps] = res;
			fixIndexJumps++;
		}
		else
		{
			BYTE holdBits = mask[index % 4 - 1] & res;
			holdBits >>= (8 - ((index % 4) * 2));
			arrOfByte[index - fixIndexJumps] = temp | holdBits;
			res <<= ((index % 4) * 2);
			temp = res;
		}

		index++;
		curr = curr->next;
	}

	if (index % 4 != 0)
		arrOfByte[index - fixIndexJumps] = temp;

	// Write bits array to bin file.
	fwrite(arrOfByte, sizeof(BYTE), numOfNodes, locationFile);

	printf("%s*File created successfully!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

	fclose(locationFile);
}

//Question 6

int checkAndDisplayPathFromFile(char* file_name) {

	FILE* pathFile = fopen(file_name, "rb");

	// Check if path file exists.
	if (pathFile == NULL)
		return NOT_FOUND;

	// Get path from file and set it to list.
	chessPosList* pathList = getPathFromFile(pathFile);

	bool isCoverAllBoard = false;
	// Check if path is a valid knight path.
	if (!checkValidKnightPath(pathList, &isCoverAllBoard))
		return NOT_VALID_PATH;

	// Show the path.
	display(pathList);

	if (isCoverAllBoard)
		return COVER_BOARD;

	return NO_COVER_BOARD;
}

bool checkValidKnightPath(chessPosList* pathList, bool* isCover) {

	bool allBoard[BOARD_SIZE][BOARD_SIZE] = { false };
	int coverAllBoardCounter = 1;

	// All possible moves of a knight
	int horizontal[KNIGHT_MOVES_SIZE] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int vertical[KNIGHT_MOVES_SIZE] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	// compare two close positions to see if the move from prev to curr is valid.
	chessPosCell* prev = pathList->head;
	chessPosCell* curr = prev->next;
	while (curr != NULL)
	{
		bool isCurrentMoveValid = false;
		for (int i = 0; i < KNIGHT_MOVES_SIZE; i++)
		{
			// find location on board for each knight's move from specified position.
			char xCoord = prev->position[0] + horizontal[i];
			char yCoord = prev->position[1] + vertical[i];

			// check if move is valid.
			if (curr->position[0] == xCoord && curr->position[1] == yCoord) {
				isCurrentMoveValid = true;

				// check if already visited this position, if not add 1 to coverAllBoardCounter.
				if (allBoard[xCoord - 'A'][yCoord - '1'] == false)
				{
					allBoard[xCoord - 'A'][yCoord - '1'] = true;
					coverAllBoardCounter++;
				}

				break;
			}
		}

		if (!isCurrentMoveValid)
			return false;

		prev = curr;
		curr = curr->next;
	}

	if (coverAllBoardCounter == BOARD_SIZE * BOARD_SIZE)
		*isCover = true;

	return true;
}

chessPosList* getPathFromFile(FILE* pathFile) {

	chessPosList* pathList = (chessPosList*)malloc(sizeof(chessPosList));
	checkMemoryAllocation(pathList);

	// Make empty list.
	pathList->head = pathList->tail = NULL;

	// Read num of positions from file.
	short int numOfPositions;
	fread(&numOfPositions, sizeof(short int), 1, pathFile);

	int numOfByte = getSizeOfBytesInFile(numOfPositions);

	BYTE keepLeftBitsMask[3] = { 0xFC, 0xF0, 0xC0 };

	BYTE temp1 = NULL;
	BYTE temp2 = NULL;
	int index = 1;

	for (int i = 0;i < numOfByte;i++) {

		BYTE currByte;
		fread(&currByte, sizeof(BYTE), 1, pathFile); // Read 8 bits from file.

		// Create position from 6 left bits.
		temp1 = currByte & keepLeftBitsMask[index % 4 - 1];

		if (index % 4 == 2 || index % 4 == 3)
		{
			temp1 >>= (((index % 4) * 2) - 2);
			temp1 |= temp2;
		}

		chessPos* pos = getPositionFromByte(temp1); // convert position from bits to chessPos.
		insertNodeToList(pos, pathList); // insert node to chessPosList.

		temp2 = (currByte << (8 - ((index % 4) * 2)));
		index++;

		// if index%4 == 0 so temp2 contain 6 bits and 2 zeros and thats a position, insert this position.
		if (index % 4 == 0 || (index + 1 % 4 == 0 && index + 1 == numOfByte))
		{
			chessPos* pos = getPositionFromByte(temp2);
			insertNodeToList(pos, pathList);
			temp2 = NULL;
			index++;
		}
	}

	fclose(pathFile);
	return pathList;
}

void insertNodeToList(chessPos* pos, chessPosList* lst) {

	chessPosCell* tempPos = createNewListNode(pos, NULL);

	if (lst->head == NULL)
		lst->head = lst->tail = tempPos;
	else
	{
		lst->tail->next = tempPos;
		lst->tail = tempPos;
	}

}

chessPos* getPositionFromByte(BYTE binPos) {

	chessPos* position = (chessPos*)malloc(sizeof(chessPos));
	checkMemoryAllocation(position);

	// Masks to get 3 left bits and 3 middle bits. (the positions)
	const BYTE mask[2] = { 0xE0, 0x1C };

	BYTE rowPos = (binPos & mask[0]) >> 5;
	BYTE colPos = (binPos & mask[1]) >> 2;

	position[0][0] = (char)rowPos + 'A';
	position[0][1] = (char)colPos + '1';

	return position;
}

