#include <stdio.h>
#include "Header.h"
#include "graphics.h"
void initial_board(int board[])
{//for initializing the array of board at the beginning of the game
	int i;
	for (i = 0; i < SQUARES; i++)
		board[i] = 0;
	board[3 * ROWS + 3] = W;
	board[3 * ROWS + 4] = B;
	board[4 * ROWS + 3] = B;
	board[4 * ROWS + 4] = W;
}

char name_of(int piece)
{
	char piecenames[4] = { '.', 'b', 'w', '?' };
	return piecenames[piece];//returns the piece element in the array of characters
}

int count(int player, int board[])
{
	int i, cnt;
	cnt = 0;
	for (i = 0; i < SQUARES; i++)
		if (board[i] == player)
			cnt++;
	return cnt;//counts each player's circles and returns them
}

void display_board(int board[])
{
	int c, r;
	printf("  ");
	for (c = 0; c < COLS; c++)
		printf("  %d", c + 1);
	printf("\n");
	for (r = 0; r < ROWS; r++)
	{
		printf("%d ", 10 * (r + 1));
		for (c = 0; c < COLS; c++)
		{
			printf(" %c ", name_of(board[r * COLS + c]));
		}
		printf("\n");
	}//printing the 10,20,30.. and 1,2,3.. and the table with the board array within the commandline
	
	printf("[%c=%d %c=%d]\n", name_of(B), count(B, board), name_of(W), count(W, board));//printing a quick result due to each round
	printf("\n"); 
	setfillstyle(SOLID_FILL, GREEN);//sets the style of filling solid
	setbkcolor(GREEN);//sets the color of the background green
	clearviewport();//keeps the drawn stuff(background color)
	for (c = 0; c <= 8; c++) {
		setcolor(BLACK);//sets the color black
		line(0, c * 50, 400, c * 50);
		line(c * 50, 0, c * 50, 400);//draws the lines vertically and horizontally
	}
	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLS; c++)
		{
			if (board[r*COLS + c] != 0) {//if a valid move is taken 
				setcolor(BLUE);//set the color blue
				circle(c * 50 + 25, r * 50 + 25, 25);//draws a circle with blue
			}
			if (board[r*COLS + c] == B) {
				setfillstyle(SOLID_FILL, BLACK);//sets filling style black if it was black player
				floodfill(c * 50 + 25, r * 50 + 25, BLUE);//starts filling the circle black from the x,y given till reaches blue color
			}
			if (board[r*COLS + c] == W) {
				setfillstyle(SOLID_FILL, WHITE);//sets filling style white if it was white player
				floodfill(c * 50 + 25, r * 50 + 25, BLUE);//starts filling the circle white from the x,y given till reaches blue color
			}
		}
	}
	setfillstyle(11,5);//sets a new style
	floodfill(500, 100, BLACK);//fills the page from the given x,y till reaches black color
	setcolor(BLACK);//sets color black
	settextstyle(BOLD_FONT, 0, 0);//sets the textstyle bold
	outtextxy(450, 10, "Oethello Game:");//prints it on the graphics window
	char str[400];//for storing the variable value
	int countblack = count(B, board);
	int countwhite = count(W, board);
	sprintf(str, "Black=%d", countblack);//black's counting
	outtextxy(450, 50, str);//in order to print a variable value the written code is necessary 
	sprintf(str, "White=%d", countwhite);//white's counting
	outtextxy(660, 50, str);//in order to print a variable value the written code is necessary 
}

int get_opponent(int player)
{//switches the turns if a legal move is made
	switch (player) {
	case B:
		return W;
	case W:
		return B;
	default:
		printf("Illegal player\n");
		return 0;
	}
}

int valid_position(int move)
{//checks valid moves by the array of board
	if (move >= 0 && move < 64)
		return 1;
	else
		return -1;
}

int check_valid_change_row(int source, int destination, int dir)
{//checks valid change rows 
	int source_row = source / COLS;
	int destination_row = destination / COLS;
	if ((dir == UP_LEFT || dir == UP || dir == UP_RIGHT) && (destination_row - source_row != -1))
		return -1;
	else if ((dir == LEFT || dir == RIGHT) && (destination_row - source_row != 0))
		return -1;
	else if ((dir == DOWN_LEFT || dir == DOWN || dir == DOWN_RIGHT) && (destination_row - source_row != 1))
		return -1;
	else
		return 1;
}

int find_bracketing_piece(int square, int player, int board[], int dir)
{//in order to check whether flip over or not
	while (valid_position(square) != -1 &&
		(board[square] == get_opponent(player) && (check_valid_change_row(square, square + dir, dir) != -1)))
	{
		square = square + dir;
	}
	if (valid_position(square) == -1)
		return -1;
	else if (board[square] == player)
		return square;
	else if (check_valid_change_row(square, square + dir, dir) == -1)
		return -1;
	else
		return -1;
}

int would_flip(int move, int player, int board[], int dir)
{//in order to flip the circles or not
	int c = move + dir;
	if ((valid_position(c) == -1) || (check_valid_change_row(move, c, dir) == -1))
		return -1;
	else if (board[c] == get_opponent(player))
		return find_bracketing_piece(c, player, board, dir);
	else
		return -1;
}

int legal_position(int move, int player, int board[])
{//checks legal positions
	int ALLDIRECTIONS[8] = { UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT };
	if (valid_position(move) == -1)
		return -1;
	if (board[move] == EMPTY) {
		int i = 0;
		while (i < 8 && (would_flip(move, player, board, ALLDIRECTIONS[i]) == -1))
			i++;
		if (i == 8)
			return -1;
		else
			return 1;
	}
	else
		return -1;
}

int any_legal_move(int player, int board[])
{//checks legal moves
	int move = 0;
	while (move < 64 && (legal_position(move, player, board) == -1))
		move++;
	if (move < 64)
		return 1;
	else
		return -1;
}

void make_flips(int move, int player, int board[], int dir)
{//flips the circles
	int bracketer, c;
	bracketer = would_flip(move, player, board, dir);
	if (bracketer != -1) {
		c = move + dir;
		while (c != bracketer)
		{
			board[c] = player;
			c = c + dir;
			if (valid_position(c) == -1)
				break;
		}
	}
}

void make_move(int move, int player, int board[])
{//checks all directions each time
	int i;
	int ALLDIRECTIONS[8] = { UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT };
	board[move] = player;
	for (i = 0; i <= 7; i++)
		make_flips(move, player, board, ALLDIRECTIONS[i]);
}

int check_endgame(int player, int board[])
{//counts the number of each player's circles and decides if the game is over if yes who the winner is or the game has no winners
	int black_score;
	int white_score;
	if (any_legal_move(player, board) == -1)
	{
		black_score = count(B, board);
		white_score = count(W, board);
		if (black_score > white_score) {
			printf("The BLACK player has won!\n");
			outtextxy(450, 130, "CONGRATS!!The black player has won!");
		}
		else if (white_score > black_score) {
			printf("The WHITE player has won!\n");
			outtextxy(450, 130, "CONGRATS!!The white player has won!");
		}
		else {
			printf("The game has no winner\n");
			outtextxy(450, 130, ":(The Game has no winner!DRAW!");
		}
		printf("Black: %d\n", black_score);
		printf("White: %d\n", white_score);
		return GAME_FINISHED;
	}
	return GAME_NOT_FINISHED;
}

int get_user_move(int player)
{//gets input from the user
	int move;
	printf("%s player's turn :\n", player == B ? "BLACK" : "WHITE");//which color turn is
	if (player == B) {
		outtextxy(450, 90, "Black player's turn:");//shows it on graphics window
	}
	else {
		outtextxy(450, 90, "White player's turn:");//shows it on graphics window
	}
	char input='i' ;
	int c = 0, r = 0;
	setcolor(YELLOW);
	rectangle(c * 50, 50 * r, c * 50 + 50, r * 50 + 50);//draws a yellow rectangular each time from0,0
	while (input != ' ') {
		input = getchar();
		setcolor(BLACK);
		rectangle(c * 50, 50 * r, c * 50 + 50, r * 50 + 50);//fills the drawn rectangular with another rectangular but black aka clears the drawn rec.
		if (input == 'w') {
			if (r > 0)//up
				r--;
		}
		if (input == 'a') {
			if (c > 0)
				c--;//left
		}
		if (input == 'd') {
			if (c < 7)
				c++;//right
		}
		if (input == 's') {
			if (r < 7)
				r++;//down
		}
			setcolor(YELLOW);
			rectangle(c * 50, 50 * r, c * 50 + 50, r * 50 + 50);//draws a new rec in the new place or last one if it was a move out of the chart
	}
		setcolor(BLACK);
		rectangle(c * 50, 50 * r, c * 50 + 50, r * 50 + 50);//aka clears the yello rec.
	return (r+1)*10+c+1;//returns the number just like before
}

int convert_user_input_to_board_index(int user_input)
{//changes or converts the input
	return (((user_input - 1) / 10) - 1) * 8 + ((user_input - 1) % 10);
}

void othello(int board[])
{
	int player = B;
	while (check_endgame(player, board) == GAME_NOT_FINISHED)//while the game is not finished
	{
		int user_input = get_user_move(player);//gets input from the user
		int move = convert_user_input_to_board_index(user_input);//checks the move
		if (legal_position(move, player, board) != -1)//if it was legal
		{
			make_move(move, player, board);//moves
			player = get_opponent(player);//chages the players turn
			display_board(board);//shows the board again
		}
		else
			printf("Invalid move. Please choose an appropriate square.\n");//if the move wasn't legal warns
	}
}