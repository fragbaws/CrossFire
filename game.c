/*
 * game.c
 *
 *  Created on: 10 Mar 2017
 *      Author: Declan
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "changingValues.h"
#include "game.h"

/*  This array contains half-clever lines which are printed whenever it is the next player's turn  */
char questions[6][100] = {
	"life has offered you lemons. What shall thou do with them?",
	"how shalt thou directeth thy fate?",
	"what shall be thy decision?",
	"quickly now! What will you do?",
	"thy wish is the algorithm's command!",
	"the enemy is upon thy borders and knows no mercy! Thou must act quickly!"
};
/*	This boolean variable is used in function runGame for more control over when a turn can end, for example when a player
 	chooses to move when there is no possibility of movement, the variable will prevent the program from simply ending the turn  */
bool turn;

/*Creates a global variable called closest_players that is an integer array of size 5, it will store the distances
 * between the current player and the all of the other players.*/
int closest_players[5];


/*function to ask the user for a board size and return the boardSize to the main function.*/
int getBoardSize(){
	int boardSize;
	printf("How big do you wish the board to be, enter one integer");
	scanf("%d", &boardSize);

	return boardSize;
}

/*Function that creates the board to be later printed.*/
void createBoard(int boardSize){

	int i, j;
	/*Using the global variable board and allocating memory of the slot size to be used later.*/
	board = malloc(boardSize * sizeof(struct slot *));

	/*Setting each column and row of the board to be the co-ordinates.*/
	for(i=0; i<boardSize; i++){
		board[i] = malloc(boardSize * sizeof(struct slot));
		for(j=0; j<boardSize; j++){
			board[i][j].row = i;
			board[i][j].column = j;
			board[i][j].player = 0;
			board[i][j].player2 = 0;
			board[i][j].player3 = 0;
			board[i][j].player4 = 0;
		}
	}

	/*Setting the up, left, down and right values of slots in the centre of the board to be the memory
	 * addresses of the corresponding variables.*/
	for(i=1; i<boardSize; i++){
		for(j=1; j<boardSize; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].left = &board[i][j-1];
			board[i][j].down = &board[i+1][j];
			board[i][j].right = &board[i][j+1];
		}
	}
	/*Setting the up, left, down and right values of slots in the first and last row of the board to
	 * be the memory addresses of the corresponding variables.*/
	for(j=1; j<boardSize-1; j++){
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		board[0][j].right = &board[0][j+1];

		board[boardSize-1][j].left = &board[boardSize-1][j-1];
		board[boardSize-1][j].up = &board[boardSize-2][j];
		board[boardSize-1][j].right = &board[boardSize-1][j+1];
	}

	/*Setting the up, left, down and right values of slots in the first and last columns of the board to
	 * be the memory addresses of the corresponding variables.*/
	for(i=1; i<boardSize-1; i++){
		board[i][0].up = &board[i-1][0];
		board[i][0].right = &board[i][1];
		board[i][0].down = &board[i+1][0];

		board[i][boardSize-1].up = &board[i-1][boardSize-1];
		board[i][boardSize-1].left = &board[i][boardSize-2];	//This line was originally board[i][boardSize-1].left = &board[boardSize-1][boardSize-2];
		board[i][boardSize-1].down = &board[i+1][boardSize-1];
	}

	//Setting the topleft slot values.
	board[0][0].right = &board[0][1];
	board[0][0].down = &board[1][0];

	//setting the topright slot values.
	board[0][boardSize-1].left = &board[0][boardSize-2];
	board[0][boardSize-1].down = &board[1][boardSize-1];

	//Setting the bottomleft slot values.
	board[boardSize-1][0].up = &board[boardSize-2][0];
	board[boardSize-1][0].right = &board[boardSize-1][1];

	//setting the bottom right slot values.
	board[boardSize-1][boardSize-1].up = &board[boardSize-2][boardSize-1];
	board[boardSize-1][boardSize-1].left = &board[boardSize-1][boardSize-2];

	return;

}

/*Function prints the board.*/
void printBoard(struct slot **board, int boardSize, struct player players[]){

	int i, j;

	for(i=0; i<boardSize; i++){
		for(j=0; j<boardSize; j++){
			//create variables for each player to be the player number stored in the corresponding variable in the slot struct.
			int pl4 = board[i][j].player4-1, pl3 = board[i][j].player3-1, pl2 = board[i][j].player2-1, pl1 = board[i][j].player-1;
			if(board[i][j].player == 0){	//If no player is in the slot then print the co-ordinates
				printf("(%d, %d - )\t ", board[i][j].row, board[i][j].column);
			}else if(board[i][j].player4 != 0){	//If a player is in the player4 variable then there are 4 people in the same slot so print out the co-ordinates and the names of the players in the slot.
				printf("(%d, %d - %s %s %s %s)\t ", board[i][j].row, board[i][j].column, players[pl1].name, players[pl2].name, players[pl3].name, players[pl4].name);
			}else if(board[i][j].player3 != 0){//If a player is in the player3 variable then there are 3 people in the same slot so print out the co-ordinates and the names of the players in the slot.
				printf("(%d, %d - %s %s %s)\t ", board[i][j].row, board[i][j].column, players[pl1].name, players[pl2].name, players[pl3].name);
			}else if(board[i][j].player2 != 0){//If a player is in the player2 variable then there are 2 people in the same slot so print out the co-ordinates and the names of the players in the slot.
				printf("(%d, %d - %s %s)\t ", board[i][j].row, board[i][j].column, players[pl1].name, players[pl2].name);
			}else if(board[i][j].player != 0){	//If a player is in the player vairble then there are 1 person in the slot so print out the co-ordinates and the names of the players in the slot.
				printf("(%d, %d - %s)\t ", board[i][j].row, board[i][j].column, players[pl1].name);
			}

		}
		printf("\n");
	}



}

/*Function to ask user to enter the number of players into the game.*/
int userNumberPlayers(){
	/*Ask the user to input how many players are playing the game and store it
	in the variable num_players*/
	printf("How many players would you like to input (max 6): ");
	scanf("%d", &num_players);
	/*If the user inputs a number greater than the maximum number of players then
	tell them that there was an error and ask them to input the number again.*/
	while(num_players>6){
		printf("\nYou can only have a maximum of 6 players. Try again.");
		printf("\nHow many players would you like to input (max 6): ");
		scanf("%d", &num_players);
	}
	return num_players;
}

/*Function to assign the players to the corresponding slots.*/
void playersToSlots(int boardSize, struct player players[]){

	int i = 0;
	int row;
	int column;

		srand(time(NULL));

		/*Iterate round num_players times.*/
		while (i < num_players){
			row = rand()%boardSize;	//Generate a random number and store it in the variable row.
			column = rand()%boardSize;	//Generate a random number and store it in the variable column.

			if((board[row][column].player3 != 0)&&(board[row][column].player4 == 0)){	//If there is a player store in the player3 variable then set the player 4 variable to be the new player number.
				board[row][column].player4 = i+1;
			}else if((board[row][column].player2 != 0)&&(board[row][column].player3 == 0)){	//If there is a player store in the player2 variable then set the player 3 variable to be the new player number.
				board[row][column].player3 = i+1;
			}else if((board[row][column].player != 0)&&(board[row][column].player2 == 0)){	//If there is a player store in the player variable then set the player 2 variable to be the new player number.
				board[row][column].player2 = i+1;
			}else if(board[row][column].player == 0){	//If there is no player stored in the player variable then assign the new player number to the player variable.
				board[row][column].player = i+1;
			}



			players[i].row = row;	// Set the players position value to be the random slot value.
			players[i].column = column;
			i++;	//Increment i by 1;
		}

		//Print out the position of each player.
		for(i=0; i<num_players; i++){
			printf("Player %d is in position, (%d, %d).\n", i, players[i].row, players[i].column);
		}
}

/*Run the game and ask each user if they wish to move or attack.*/
void runGame(struct player players[], struct slot slots[], int num_players, int num_slots, int boardSize){

	int i, ans;	//Declaring local variables

	/*	Print the board initially at the start of the game  */
	printf("The Board:\n");

	printBoard(board, boardSize, players);

	printf("\nThe battle begins now!\n\n");

	srand(time(NULL));
	for (i = 0; i < num_players; i++){
		/*  Ask the next player to choose an action */
		printf("%s, %s\n", players[i].name, questions[rand() % 6]);	// 6 is the number of questions in the questions array defined in game.h
		printf("1: Move\n");		// Call the move function to move the player to a nearby slot
		printf("2: Attack\n");		// Call the attack function to attack a nearby player
		scanf("%d", &ans);			// Store the input in the integer variable ans

		/*If the answer is not a valid input then print an error and ask for a new input.*/
		while ((ans != 1) && (ans != 2) && (ans != 0)){
			printf("I'm sorry? I do not understand thy language.\n");
			scanf("%d", &ans);
		}

		/*Switch statement compares the answer input and calls the function that corresponds to the input.*/
		switch (ans){
		case 1:
			movePlayers(board, players, boardSize, i);
			printBoard(board, boardSize, players);
			break;
		case 2:

			break;
		case 0:
			break;
		}
	}

	/*	Print the board and player details for the final results.  */
	printf("Final Board:\n");
	printBoard(board, boardSize, players);
	//printFinalResults(players, slots, num_players);
}

/*Check the players current position and then call the correct function based on that position.*/
void movePlayers(struct slot **board, struct player players[], int boardSize, int current_player){
	int position;
	if((players[current_player].row == 0)&&(players[current_player].column == 0)){	//If the player is in the topleft position then set position to 1 and call the twoDirections function.
		printf("\nPlayer is in top left.\n");
		position = 1;
		twoDirections(board, players, boardSize, current_player, position);
	}else if((players[current_player].row == 0) && (players[current_player].column == boardSize-1)){	//If the player is in the topright position then set position to 2 and call the twoDirections function.
		printf("\nPlayer is in top right.\n");
		position = 2;
		twoDirections(board, players, boardSize, current_player, position);
	}else if((players[current_player].row == boardSize-1) && (players[current_player].column == 0)){	//If the player is in the bottomleft position then set position to 3 and call the twoDirections function.
		printf("\nPlayer is in bottom left.\n");
		position = 3;
		twoDirections(board, players, boardSize, current_player, position);
	}else if((players[current_player].row == boardSize-1) && (players[current_player].column == boardSize-1)){	//If the player is in the bottomright position then set position to 4 and call the twoDirections function.
		printf("\nPlayer is in bottom right.\n");
		position = 4;
		twoDirections(board, players, boardSize, current_player, position);
	}else if(players[current_player].row == 0){	//If the player is in the top row then set position to be 5 and call the threeDirections function.
		printf("\nPlayer is in first row.\n");
		position = 5;
		threeDirections(board, players, boardSize, current_player, position);
	}else if(players[current_player].row == boardSize-1){	//If the player is in the bottom row then set position to be 6 and call the threeDirections function.
		printf("\nPlayer is in last row.\n");
		position = 6;
		threeDirections(board, players, boardSize, current_player, position);
	}else if(players[current_player].column == 0){	//If the player is in the first column then set position to be 7 and call the threeDirections function.
		printf("\nPlayer is in first column.\n");
		position = 7;
		threeDirections(board, players, boardSize, current_player, position);
	}else if(players[current_player].column == boardSize-1){	//If the player is in the last column then set position to be 8 and call the threeDirections function.
		printf("\nPlayer is in last column.\n");
		position = 8;
		threeDirections(board, players, boardSize, current_player, position);
	}else{	//Otherwise the player is in the centre of the board so call the fourDirections function.
		printf("\nPlayer is in centre of board.\n");
		fourDirections(board, players, boardSize, current_player);
	}


}

/*Function that allows the player to move in any twoDirections, called if the player is in the corner of the board.*/
void twoDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position){

	int choice, row, column;

		printf("Where would you like to move %s?\n", players[current_player].name);

		if(position == 1){	//If the player is in position topRight then ask where they wish to move.
			printf("2: Move right.\n");
			printf("3: Move down.\n");
			scanf("%d", &choice);
		}else if(position == 2){	//If the player is in position topLeft then ask where they wish to move.
			printf("3: Move down.\n");
			printf("4: Move left.\n");
			scanf("%d", &choice);
		}else if(position == 3){	//If the player is in position bottomLeft then ask where they wish to move.
			printf("1: Move up.\n");
			printf("2: Move right.\n");
			scanf("%d", &choice);
		}else if(position == 4){	//If the player is in position bottomRight then ask where they wish to move.
			printf("1: Move up.\n");
			printf("4: Move left.\n");
			scanf("%d", &choice);
		}

		row = players[current_player].row;	//Set the row variable to be the current players current row position.
		column = players[current_player].column;	//Set the column variable to be the current players current column position.

		/*Based on where the player wishes to move the function will run the corresponding code from the switch statement.*/
	switch(choice){
	case 1:
		if(board[row][column].up->player3 != 0){	//If a third player is in the slot above then add the players position to the player4 variable.
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){	//If a second player is in the slot above then add the players position to the player3 variable.
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){	//If a player is in the slot above then add the players position to the player2 variable.
			board[row][column].up->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].up->player = current_player+1;
		}
		board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row-1;	//set the new value of the players row variable.
		break;

	case 2:
		if(board[row][column].up->player3 != 0){	//If a third player is in the slot above then add the players position to the player4 variable.
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){	//If a second player is in the slot above then add the players position to the player3 variable.
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){	//If a player is in the slot above then add the players position to the player2 variable.
			board[row][column].up->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].up->player = current_player+1;
		}
		board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row-1;	//set the new value of the players row variable.
		break;

	case 3:
		if(board[row][column].down->player3 != 0){	//If a third player is in the slot below then add the players position to the player4 variable.
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){	//If a second player is in the slot below then add the players position to the player3 variable.
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){	//If a player is in the slot below then add the players position to the player2 variable.
			board[row][column].down->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].down->player = current_player+1;
		}
		board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row-1;	//set the new value of the players row variable.
		break;
	case 4:
		if(board[row][column].left->player3 != 0){	//If a third player is in the slot to the left of then add the players position to the player4 variable.
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){	//If a second player is in the slot to the left of then add the players position to the player3 variable.
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){	//If a player is in the slot to the left of then add the players position to the player2 variable.
			board[row][column].left->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].left->player = current_player+1;
		}
		board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row-1;	//set the new value of the players row variable.
		break;
	}
}

/*Function that allows the player to move in any three directions, called if the player is along the side of the board.*/
void threeDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position){
	int choice, row, column;

	printf("Where would you like to move %s?\n", players[current_player].name);

	if(position == 8){
		printf("1: Move up.\n");
		printf("3: Move down.\n");
		printf("4: Move left.\n");
		scanf("%d", &choice);
	}else if(position == 7){
		printf("1: Move to the left of.\n");
		printf("2: Move right.\n");
		printf("3: Move down.\n");
		scanf("%d", &choice);
	}else if(position == 6){
		printf("1: Move up.\n");
		printf("2: Move right.\n");
		printf("4: Move left.\n");
		scanf("%d", &choice);
	}else if(position == 5){
		printf("2: Move right.\n");
		printf("3: Move down.\n");
		printf("4: Move left.\n");
		scanf("%d", &choice);
	}

	row = players[current_player].row;
	column = players[current_player].column;


	switch(choice){
	case 1:
		if(board[row][column].up->player3 != 0){
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){
			board[row][column].up->player2 = current_player+1;
		}else{
			board[row][column].up->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;

	case 2:
		if(board[row][column].right->player3 != 0){
			board[row][column].right->player4 = current_player+1;
		}else if(board[row][column].right->player2 != 0){
			board[row][column].right->player3 = current_player+1;
		}else if(board[row][column].right->player != 0){
			board[row][column].right->player2 = current_player+1;
		}else{
			board[row][column].right->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;

	case 3:
		if(board[row][column].down->player3 != 0){
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){
			board[row][column].down->player2 = current_player+1;
		}else{
			board[row][column].down->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;
	case 4:
		if(board[row][column].left->player3 != 0){
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){
			board[row][column].left->player2 = current_player+1;
		}else{
			board[row][column].left->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;
	}
}

/*Function that allows the player to move in any four directions, called if the player is in the centre of the board.*/
void fourDirections(struct slot **board, struct player players[], int boardSize, int current_player){
	int choice, row, column;
	printf("Where would you like to move %s?\n", players[current_player].name);
	printf("1: Move up.\n");
	printf("2: Move right.\n");
	printf("3: Move down.\n");
	printf("4: Move left.\n");
	scanf("%d", &choice);
	row = players[current_player].row;
	column = players[current_player].column;


	switch(choice){
	case 1:
		if(board[row][column].up->player3 != 0){
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){
			board[row][column].up->player2 = current_player+1;
		}else{
			board[row][column].up->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;

	case 2:
		if(board[row][column].right->player3 != 0){
			board[row][column].right->player4 = current_player+1;
		}else if(board[row][column].right->player2 != 0){
			board[row][column].right->player3 = current_player+1;
		}else if(board[row][column].right->player != 0){
			board[row][column].right->player2 = current_player+1;
		}else{
			board[row][column].right->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;

	case 3:
		if(board[row][column].down->player3 != 0){
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){
			board[row][column].down->player2 = current_player+1;
		}else{
			board[row][column].down->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;
	case 4:
		if(board[row][column].left->player3 != 0){
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){
			board[row][column].left->player2 = current_player+1;
		}else{
			board[row][column].left->player = current_player+1;
		}
		board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		break;
	}

}

/*This function changes the capabilities based on the strength values of the current player and the attacked player.*/
void attackPlayer(struct player players[], int attacked_player, int current_player){

	attacked_player-=1; // Decrement attacked_player by 1.

	/*If the attacked_player strength is less than or equal to 70 then use the formula to change the attacked_player's life capability.
	 * Else if the attacked_player's strength is greater than 70 then use the formula to change the current_player's life capability.*/
	if(players[attacked_player].strength <=70){
		players[attacked_player].life = (players[attacked_player].life - (0.5 * players[attacked_player].strength));
	}else if(players[attacked_player].strength >70){
		players[current_player].life = (players[current_player].life - (0.3 * players[attacked_player].strength));
	}
}

void hillMovement(struct player players[], int current_player){
	int j;

	j = current_player;

	if(players[j].dexterity<50){
		if(players[j].strength<10){
			players[j].strength = 0;
		}else{
			players[j].strength -=10;
		}
	}else if(players[j].dexterity >=60){
		players[j].strength +=10;
	}
}
/*This function changes the player capabilities if the player has moved to a city slot.*/
void cityMovement(struct player players[], int current_player){
	int j;
	j = current_player;

	if(players[j].smartness<=50){
		if(players[j].dexterity<10){
			players[j].dexterity = 0;
		}else{
			players[j].dexterity -=10;
		}
	}else if(players[j].smartness >60){
		players[j].magic +=10;
	}
}

/*This function prints the final player values, e.g. name, player type and life points.*/
void printFinalResults(struct player players[], struct slot slots[], int num_players){

	int i;

	printf("----------------------------------------------------\n");
	printf("\t\tFinal Player Values\n");
	printf("----------------------------------------------------\n");
		/*Prints the players name and type.*/
		for(i=0; i<num_players; i++){
			printf("%s ", players[i].name);
			printf("(%s, %d)\n", players[i].type, players[i].life);
		}
		printf("----------------------------------------------------\n");
		printf("\n");
}
