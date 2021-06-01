#ifndef _CHESS_MENU
#define _CHESS_MENU

#include "head.h"

void chessMenu();
void menuOptionOne(chessPos** userPos);
pathTree menuOptionTwo(chessPos* userPos);
void menuOptionThree(pathTree* pt, chessPosList* lst);
void menuOptionFour(pathTree* pt, chessPosList* lst);
void menuOptionFive();

#endif