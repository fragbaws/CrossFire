#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define DESCRIPTION 30
#define TYPE 3

char slot_type[TYPE][DESCRIPTION] = {"Ground", "Hill", "City"};  // Global variable which can be accessed by any function

struct players
{
	char type[7]; 											// player's type
	char name[10];											// player's name
	int life_points;										// player's life points
	int smartness, strength, magic_skills, luck, dexterity; // player's capabilities
	int position[2];										// player's position in the slot
	int shift_slot;											// keeps track if player moved or not, if moved shift_slot == 1, if not shift_slot == 0
};

struct slot
{
	char slot_type[DESCRIPTION];	// slot type

	int row; 						// row number
	int column;						// column number
	int player_no;					// number of player in the slot position

	struct slot *left; 				//	adjacent left slot
	struct slot *right;				//	adjacent right slot
	struct slot *up;				//	adjacent up slot
	struct slot *down;				// 	adjacent down slot
}slot;


void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);	// creates the board
void printBoard(int boardSize);																									// prints the details of each slot position
void randomTypeBoard(int boardSize);																							// assigns a random slot type to each slot position

struct slot **board; // global variable for easy access

int main(void)
{

	setvbuf(stdout, NULL, _IONBF, 0); // fixes eclipse buffer errors on WINDOWS OS

	struct slot *upRight;
	struct slot *upLeft;
	struct slot *downLeft;
	struct slot *downRight;

	int boardSize = 7;

	createBoard(boardSize, &upLeft, &upRight, &downLeft, &downRight);
	randomTypeBoard(boardSize);
	printBoard(boardSize);


	return 0;
}


void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight)
{
	int i,j;
	//The board is represented as a pointer of pointer to slots
	//This allocates in memory the space for the pointers to each row of the board
	board = malloc(boardSize * sizeof(struct slot *));


	for(i = 0; i < boardSize; i++){
		//This allocates in memory the space for the slots in each row of the board
		board[i] = malloc(boardSize * sizeof(struct slot));

		//For each slot it sets up the row and column number
		for( j=0;j < boardSize; j++){
			board[i][j].row = i;
			board[i][j].column = j;
		}
	}

	//It sets up the adjacent slots for the slots that are
	//not at the borders. These slots have 4 adjacent elements
	for( i =1; i< boardSize-1; i++){
		for(j=1;j < boardSize-1; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last row, except the slots at the edges.
	//
	for(j = 1; j < boardSize -1; j++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on top of them
		// because they are on the first row of the board
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on down them
		// because they are on the last row of the board
		board[boardSize - 1][j].right = &board[boardSize - 1][j+1];
		board[boardSize -1][j].left = &board[boardSize - 1][j-1];
		board[boardSize - 1][j].up = &board[boardSize - 2][j];
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last column, except the slots at the edges.
	//
	for(i = 1; i < boardSize -1; i++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on the left
		// because they are on the first column of the board
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];
		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on the right
		// because they are on the last column of the board
		board[i][boardSize-1].left = &board[boardSize - 1][boardSize-2];
		board[i][boardSize -1].up = &board[i -1][boardSize-1];
		board[i][boardSize -1].down = &board[i+1][boardSize -1];
	}

	//It sets up the adjacent slots for the slot at position (0,0).
	//This only has only 2 adjacent slots: right and down
	board[0][0].right = &board[0][1];
	board[0][0].down = &board[1][0];

	//It sets up the adjacent slots for the slot at position (0,boardSize -1).
	//This only has only 2 adjacent slots: left and down
	board[0][boardSize-1].left = &board[0][boardSize-2];
	board[0][boardSize -1].down = &board[1][boardSize -1];

	//It sets up the adjacent slots for the slot at position (boarSize -1 ,0).
	//This only has only 2 adjacent slots: up and right
	board[boardSize -1][0].right = &board[boardSize -1][1];
	board[boardSize -1][0].up = &board[boardSize -2][0];

	//It sets up the adjacent slots for the slot at position (boarSize -1 ,boardSize-1).
	//This only has only 2 adjacent slots: up and left
	board[boardSize - 1][boardSize-1].up = &board[boardSize-2][boardSize-1];
	board[boardSize - 1][boardSize -1].left = &board[boardSize -1][boardSize -2];

	//assigns a pointer to slot at position (0, 0)
	*upLeft = &board[0][0];
	//assigns pointer of pointer to slot at position (0, boardSize -1)
	*upRight = &board[0][boardSize -1];
	//assigns pointer of pointer to slot at position ( boardSize -1,)
	*downLeft = &board[boardSize -1][0];
	//assigns pointer of pointer to slot at position (boardSize -1, boardSize -1)
	*downRight = &board[boardSize -1][boardSize -1];

}

void printBoard(int boardSize)
{
	for(int i=0;i<boardSize;i++)
	{
		for(int j=0;j<boardSize;j++)
		{
			printf("[(%d,%d),%s]\t",i,j, board[i][j].slot_type);
		}
		printf("\n");
	}
}

void randomTypeBoard(int boardSize)
{
	// Assigning a random slot type to each slot position of the board

	srand((unsigned int)time(NULL));
	int rdm;

	for(int i=0;i<boardSize;i++)
	{
		for(int j=0;j<boardSize;j++)
		{
			rdm = rand()%3;
			strcpy(board[i][j].slot_type, slot_type[rdm]);
		}
	}
}
