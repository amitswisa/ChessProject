#ifndef _FILE_OPERTAIONS
#define _FILE_OPERTAIONS

#include "head.h"

// Question 5
void saveListToBinFile(char* file_name, chessPosList* pos_list);
short int numOfNodesInList(chessPosCell* node);
int getSizeOfBytesInFile(short int numOfNodes);

// Question 6
int checkAndDisplayPathFromFile(char* file_name);
chessPosList* getPathFromFile(FILE* pathFile);
chessPos* getPositionFromByte(BYTE binPos);
void insertNodeToList(chessPos* pos, chessPosList* lst);
bool checkValidKnightPath(chessPosList* pathList, bool* isCover);

#endif