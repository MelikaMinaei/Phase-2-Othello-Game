#include <stdio.h>
#include "Header.h"
#include "graphics.h"


int main()
{
	initwindow(1360, 700, "oethello", 0, 0);//opens the graphics window named oethello
	int board[SQUARES];//the board array[64]
	initial_board(board);//initlializes
	display_board(board);//shows the first board
	othello(board);//starts the loop
	getchar();//in order to show the graphics window when game ends,,not just close everything really fast
	return 0;
}