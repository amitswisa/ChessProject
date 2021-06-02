#include "chessMenu.h"
#include "knightClass.h"
#include "fileOps.h"

void chessMenu() {

	// Chess project menu.
	chessPos* userPos = NULL;
	pathTree pt;
	pt.root = NULL;
	chessPosList lst;
	lst.head = NULL;

	int userChoice = 0;
	while (userChoice != 6)
	{
		printf("\n1. Enter a knight's starting position\n2. Create all possible knight paths\n3. Find a knight path covering all board\n4. Save knight path covering all board to file\n5. Load and display path from file\n6. Exit\n");
		printf("%s# Choose option:%s ", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
		scanf("%d", &userChoice); // Get user choice.
		clearBuffer();

		// verify choice.
		if (userChoice <= 0 || userChoice >= 7)
		{
			printf("%s*Error: please enter valid choice!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			continue;
		}

		// check if user chose option that needs starting knight position.
		if (userPos == NULL && (userChoice >= 2 && userChoice <= 4))
		{
			printf("%s*Error: please enter knight's starting position first! (option 1)%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			continue;
		}

		// if user chose option 3 or 4 and pathTree isnt build yet --> build it.
		if ((userChoice == 3 || userChoice == 4) && pt.root == NULL)
		{
			pt = menuOptionTwo(userPos);
		}

		// Option of menu.
		switch (userChoice)
		{
		case 1: {
			freeAll(&pt, &lst);
			menuOptionOne(&userPos);
			break;
		}
		case 2: {
			if (pt.root == NULL)
				pt = menuOptionTwo(userPos);
			else
				printf("%s*pathTree from your current position is already created.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			break;
		}
		case 3: {
			menuOptionThree(&pt, &lst);
			break;
		}
		case 4: {
			menuOptionFour(&pt, &lst);
			break;
		}
		case 5: {
			menuOptionFive();
			break;
		}
		case 6: {
			printf("\nGoodbye!\n");
			freeAll(&pt, &lst);
			free(userPos);
			break;
		}
		}

	}
}

void menuOptionOne(chessPos** userPos) {

	if (*userPos != NULL) {
		free(*userPos);
	}

	chessPos* temp = (chessPos*)malloc(sizeof(chessPos));
	checkMemoryAllocation(temp);

	int colPos;
	char rowPos;
	bool isValidChoice = false;
	printf("Please enter knight's starting position (Example: A 2): ");
	while (!isValidChoice)
	{
		scanf(" %c %d", &rowPos, &colPos);

		isValidChoice = isInsideBoardBoundries(rowPos, colPos + '0');
		if (!isValidChoice)
			printf("%s*Error: position is not valid!%s\nPlease enter new knight's starting position (Example: A 2): ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		else
		{
			temp[0][0] = rowPos;
			temp[0][1] = colPos + '0';
			isValidChoice = true;
		}
	}

	printf("%s*Position received successfully!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);

	*userPos = temp;
}

pathTree menuOptionTwo(chessPos* userPos) {
	pathTree pt = findAllPossibleKnightPaths(userPos);
	printf("%s*pathTree created successfully!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
	return pt;
}

void menuOptionThree(pathTree* pt, chessPosList* lst) {

	if (lst->head != NULL)
		freeList(lst);

	chessPosList* temp = findKnightPathCoveringAllBoard(pt);
	if (temp->head == NULL)
	{
		printf("%s*Sorry, there is no path covering all board from current position.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	}
	else {
		printf("%s*List with path that covers all board created successfully!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
	}

	*lst = *temp;
	free(temp);
}

void menuOptionFour(pathTree* pt, chessPosList* lst) {

	if (lst->head == NULL)
		menuOptionThree(pt, lst);

	if (lst->head != NULL)
	{
		char* fileName = getFileNameFromUser();

		saveListToBinFile(fileName, lst);

		free(fileName);
	}
}

void menuOptionFive() {

	char* fileName = getFileNameFromUser();

	if (isFileEmpty(fileName)) {

	}

	printf("\n");
	int res = checkAndDisplayPathFromFile(fileName);

	switch (res)
	{
	case NOT_FOUND: {
		printf("%s*Error: '%s' file does not exist.%s\n", ANSI_COLOR_RED, fileName, ANSI_COLOR_RESET);
		break;
	}
	case NOT_VALID_PATH: {
		printf("%s*Error: path mention in file isnt a valid path.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		break;
	}
	case COVER_BOARD: {
		printf("%s*File loaded successfully and path covers all board.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
		break;
	}
	case NO_COVER_BOARD: {
		printf("%s*File loaded successfully but path isnt covering all board.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
		break;
	}
	}

	free(fileName);
}
