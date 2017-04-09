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
#include <math.h>
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

/*Function that creates the board to be later printed.*/
void createBoard(int boardSize){

	int i, j, k;
	/*Using the global variable board and allocating memory of the slot size to be used later.*/
	board = malloc(boardSize * sizeof(struct slot *));
	foundSlots = malloc(boardSize * sizeof(struct slot *));

	/*Setting each column and row of the board to be the co-ordinates.*/
	for(i=0; i<boardSize; i++){
		board[i] = malloc(boardSize * sizeof(struct slot));
		foundSlots[i] = malloc(boardSize * sizeof(struct slot));
		for(j=0; j<boardSize; j++){
			board[i][j].row = i;
			board[i][j].column = j;
			foundSlots[i][j].row = i;
			foundSlots[i][j].column = j;
			board[i][j].no_of_players = 0;
			foundSlots[i][j].no_of_players = 0;
			for(k=0; k<num_players; k++){
				board[i][j].player_nos[k] = -1;
				foundSlots[i][j].player_nos[k] = -1;
			}
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

    int i, j, k, count=0;
    int player[6];

    for(i=0; i<boardSize; i++){
    	for(j=0; j<boardSize; j++){ //Nested for loop to loop round each element in the board.

                for(k=0; k<num_players; k++){	//For loop to loop around the player_nos array in the board slot
                    if(board[i][j].player_nos[k] != -1){	//Check if there is a player in the board.
                        player[count] = board[i][j].player_nos[k]-1;	//Copy the player numbers into the new array player and count how many players are in the array.
                        count++;
                    }
                }

                if(count == 0){	//If there were no players counted then print the slot type, row and column.
                    printf("[%-7s (%d, %d)]\t", board[i][j].type_of_slot, board[i][j].row, board[i][j].column);
                }else if(count == 1){	//If there was one player counted then print the slot type, row, column and player number.
                    printf("[%-7s (%d, %d) - %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]].name);
                }else if(count == 2){	//If there was two players counted then print the slot type, row, column and player numbers.
                    printf("[%-7s (%d, %d) - %s %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]].name, players[player[1]].name);
                }else if(count == 3){	//If there was three players counted then print the slot type, row, column and player numbers.
                    printf("[%-7s (%d, %d) - %s %s %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]-1].name, players[player[1]-1].name, players[player[2]-1].name);
                }else if(count == 4){	//If there was four players counted then print the slot type, row, column and player numbers.
                    printf("[%-7s (%d, %d) - %s %s %s %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]-1].name, players[player[1]-1].name, players[player[2]-1].name, players[player[3]-1].name);
                }else if(count == 5){	//If there was five players counted then print the slot type, row, column and player numbers.
                    printf("[%-7s (%d, %d) - %s %s %s %s %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]-1].name, players[player[1]-1].name, players[player[2]-1].name, players[player[3]-1].name, players[player[4]-1].name);
                }else if(count == 6){	//If there was six players counted then print the slot type, row, column and player numbers.
                    printf("[%-7s (%d, %d) - %s %s %s %s %s %s]\t",board[i][j].type_of_slot, board[i][j].row, board[i][j].column, players[player[0]-1].name, players[player[1]-1].name, players[player[2]-1].name, players[player[3]-1].name, players[player[4]-1].name, players[player[5]-1].name);
                }

                count = 0;
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

			/*if((board[row][column].player3 != 0)&&(board[row][column].player4 == 0)){	//If there is a player store in the player3 variable then set the player 4 variable to be the new player number.
				board[row][column].player4 = i+1;
			}else if((board[row][column].player2 != 0)&&(board[row][column].player3 == 0)){	//If there is a player store in the player2 variable then set the player 3 variable to be the new player number.
				board[row][column].player3 = i+1;
			}else if((board[row][column].player != 0)&&(board[row][column].player2 == 0)){	//If there is a player store in the player variable then set the player 2 variable to be the new player number.
				board[row][column].player2 = i+1;
			}else if(board[row][column].player == 0){	//If there is no player stored in the player variable then assign the new player number to the player variable.
				board[row][column].player = i+1;
			}*/

			board[row][column].player_nos[i] = i+1;	//Assign the players number to the corresponding element in the array of the slot.
			board[row][column].no_of_players += 1;	//Update the number of players in the slot.

			players[i].row = row;	// Set the players position value to be the random slot value.
			players[i].column = column;
			i++;	//Increment i by 1;
		}

		//Print out the position of each player.
		for(i=0; i<num_players; i++){
			printf("Player %d is in position, (%d, %d).\n", i+1, players[i].row, players[i].column);
		}
		printf("\n");
}

/*findSlots is a recursive function that finds the range of slots between the range or 1 and 5 for the distant attack.*/
void findSlots(int boardSize, struct player players[], int exploredRow, int exploredColumn, int currentRow, int currentColumn, int reqDistance, int current_player){
	int c=0, currDistance=0;

	int rowDistance = (board[exploredRow][exploredColumn].row - board[currentRow][currentColumn].row);	//Find the distance of the slots row that the player is currently in and the slots row that the function is currently checking.
	int columnDistance = (board[exploredRow][exploredColumn].column - board[currentRow][currentColumn].column); //Find the distance of the slots column that the player is currently in and the slots column that the function is currently checking.
	currDistance = abs(rowDistance) + abs(columnDistance);	//Calculate the total distance from the slot that the player is currently in and the slot that is being checked by taking the absolute value of both the rowDistance and the columnDistance and adding together.
	//printf("CurrDistance %d\n", currDistance);


	if((currDistance>1)&&(currDistance<5)){	//If the distance calculated is within the range.

		if(board[exploredRow][exploredColumn].no_of_players > 0){	//If the slot being checked has players stored in it.
			//printf("There are %d people in slot (%d %d)\n", board[exploredRow][exploredColumn].no_of_players, exploredRow, exploredColumn);
			for(c=0; c<num_players; c++){
				if(board[exploredRow][exploredColumn].player_nos[c] >0){	//If the player array of the slot has a value that is greater than 0 then there is someone
					//printf("%d, ", board[exploredRow][exploredColumn].player_nos[c]);
					playersInRange[playersInRangeCounter] = board[exploredRow][exploredColumn].player_nos[c];	//Copy the player that is stored in the array to a new array called playersInRange to be accessed later to be printed.
					//printf("Copied value %d into the array.",board[exploredRow][exploredColumn].player_nos[c]);
					playersInRangeCounter++;	//Update the counter.
				}
			}
			//printf("\n");
		}
		//printf("\n\n");


		if((board[exploredRow][exploredColumn].right != NULL)&&(count%7 !=0)){	//If the slot being explored has a slot to the right and the count variable doesn't have a remainder of 0 when divided by 7.
			//printf("GOING RIGHT\t");
			//printf("Count: %d\n", count);
			count++;
			findSlots(boardSize, players, board[exploredRow][exploredColumn].row, board[exploredRow][exploredColumn].right->column, currentRow, currentColumn, 2, current_player);	//Call the find slots function.
		}else if(count == boardSize*boardSize){	//If the count variable is the same as the last slot in the board.
			//printf("LAST SLOT\n");
			playerToAttack(players, boardSize, currentRow, currentColumn, current_player);	//Call the playerToAttack function.
			return;
		}else if(count%7 == 0){	//If the count variable has a remainder of 0 when divided by 7 (In the last slot of the row)
			//printf("GOING DOWN AND TO THE START \t");
			count++;
			findSlots(boardSize, players, board[exploredRow][exploredColumn].down->row, 0, currentRow, currentColumn, 2, current_player);	//call the findSlots function and move to the next row and first column.
		}
	}else{
		if((board[exploredRow][exploredColumn].right != NULL)&&(count%7 !=0)){//If the slot being explored has a slot to the right and the count variable doesn't have a remainder of 0 when divided by 7.
			//printf("GOING RIGHT\t");
			//printf("Count: %d\n", count);
			count++;
			findSlots(boardSize, players, board[exploredRow][exploredColumn].row, board[exploredRow][exploredColumn].right->column, currentRow, currentColumn, 2, current_player);//Call the find slots function.
		}else if(count == boardSize*boardSize){//If the count variable is the same as the last slot in the board.
			//printf("LAST SLOT\n");
			playerToAttack(players, boardSize, currentRow, currentColumn, current_player);	//Call the playerToAttack function.
			return;
		}else if(count%7 == 0){	//If the count variable has a remainder of 0 when divided by 7 (In the last slot of the row)
			//printf("GOING DOWN AND TO THE START \t");
			count++;
			findSlots(boardSize, players, board[exploredRow][exploredColumn].down->row, 0, currentRow, currentColumn, 2, current_player);	//call the findSlots function and move to the next row and first column.
		}
	}
}

/*Prints out the players the current player can attack.*/
void playerToAttack(struct player players[], int boardSize, int currentRow, int currentColumn, int current_player){
	int i, choice=0, peopleInArray=0;

	printf("Here are you options to attack: \n");

	for(i=0; i<num_players; i++){
		if(playersInRange[i] > 0){	//If the playersInRange array's element is greater than 0 then update the people in the array counter.
			peopleInArray = 1;
		}
	}

	if(peopleInArray == 1){	//If the poepleInArray counter is equal to 1.
		for(i=0; i<num_players; i++){
			if(playersInRange[i] > 0){
				printf("Enter %d: to attack player %s\n", playersInRange[i], players[playersInRange[i]-1].name);	//Print out the option that the player can attack.
			}
		}
		scanf("%d", &choice);	//store the user input into the choice variable.

		switch(choice){	//Call the attackDistant function depending on the input from the user.
			case 1:
				printf("attacking player %d\n", 1);
				attackPlayerDistant(players, 0, current_player);
				break;
			case 2:
				printf("attacking player %d\n", 2);
				attackPlayerDistant(players, 1, current_player);
				break;
			case 3:
				printf("attacking player %d\n", 3);
				attackPlayerDistant(players, 2, current_player);
				break;
			case 4:
				printf("attacking player %d\n", 4);
				attackPlayerDistant(players, 3, current_player);
				break;
			case 5:
				printf("attacking player %d\n", 5);
				attackPlayerDistant(players, 4, current_player);
				break;
			case 6:
				printf("attacking player %d\n", 6);
				attackPlayerDistant(players, 5, current_player);
				break;
		}
	}else{
		printf("There are no players close by, you must move.\n");
		movePlayers(board, players, boardSize, current_player);
	}


}

/
void playerToAttackNear(struct player players[], int boardSize, int currentRow, int currentColumn, int current_player){
	int i, j, playersInSlots[6], ans, playersNear;

	for(i=0; i<num_players; i++){
		playersInSlots[i] = -1;
	}

	for(j=0; j<num_players; j++){
		if(playersInSlots[j] > 0){
			playersNear = 1;
		}
	}

	for(i=0; i<5; i++){
		if(i==0){
			if(board[currentRow][currentColumn].up->no_of_players > 0){
				for(j=0; j<num_players; j++){
					if(board[currentRow][currentColumn].up->player_nos[j] != -1){
						playersInSlots[j] = board[currentRow][currentColumn].up->player_nos[j];
					}
				}
			}
		}

		if(i==1){
			if(board[currentRow][currentColumn].right->no_of_players > 0){
				for(j=0; j<num_players; j++){
					if(board[currentRow][currentColumn].right->player_nos[j] != -1){
						playersInSlots[j] = board[currentRow][currentColumn].right->player_nos[j];
					}
				}
			}
		}

		if(i==2){
			if(board[currentRow][currentColumn].right->no_of_players > 0){
				for(j=0; j<num_players; j++){
					if(board[currentRow][currentColumn].right->player_nos[j] != -1){
						playersInSlots[j] = board[currentRow][currentColumn].right->player_nos[j];
					}
				}
			}
		}

		if(i==3){
			if(board[currentRow][currentColumn].right->no_of_players > 0){
				for(j=0; j<num_players; j++){
					if(board[currentRow][currentColumn].right->player_nos[j] != -1){
						playersInSlots[j] = board[currentRow][currentColumn].right->player_nos[j];
					}
				}
			}
		}

		if(i==4){
			if(board[currentRow][currentColumn].no_of_players > 1){
				for(j=0; j<num_players; j++){
					if((board[currentRow][currentColumn].player_nos[j] != -1) || (board[currentRow][currentColumn].player_nos[j] != current_player)){
						playersInSlots[j] = board[currentRow][currentColumn].player_nos[j];
					}
				}
			}
		}

	}


	if(playersNear == 1){
		for(i=0; i<num_players; i++){
			if(playersInSlots[i] != -1){
				printf("Enter %d to attack %s.\n", playersInSlots[i], players[playersInSlots[i]].name);
			}
		}
		scanf("%d", &ans);

		switch(ans){
		case 1:
			attackPlayerNear(players, 1, current_player);
			break;
		case 2:
			attackPlayerNear(players, 2, current_player);
			break;
		case 3:
			attackPlayerNear(players, 3, current_player);
			break;
		case 4:
			attackPlayerNear(players, 4, current_player);
			break;
		case 5:
			attackPlayerNear(players, 5, current_player);
			break;
		case 6:
			attackPlayerNear(players, 6, current_player);
			break;

		}
	}else{
		printf("There are no players near, you must move.\n");
		movePlayers(board, players, boardSize, current_player);
		return;
	}

}

void typetoSlot(int boardSize)
{
    srand((unsigned)time(NULL));

    char slot_types[3][7]= {"Ground","Hill","City"};

    int random;

    for(int i=0;i<boardSize;i++)
    {
        for(int j=0;j<boardSize;j++)
        {
            random = rand()%3;
            strcpy(board[i][j].type_of_slot, slot_types[random]);
        }
    }
}


/*Run the game and ask each user if they wish to move or attack.*/
void runGame(struct player players[], struct slot slots[], int num_players, int num_slots, int boardSize){

	int i, j, c, k, ans, ansAttack, ansMagicAttack = 1;	//Declaring local variables

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
		printf("3: Leave Game\n");
		scanf("%d", &ans);			// Store the input in the integer variable ans

		/*If the answer is not a valid input then print an error and ask for a new input.*/
		while ((ans != 1) && (ans != 2) && (ans != 3)){
			printf("I'm sorry? I do not understand thy language.\n");
			scanf("%d", &ans);
		}

		/*Switch statement compares the answer input and calls the function that corresponds to the input.*/
		switch (ans){
		case 1:
			movePlayers(board, players, boardSize, i);
			//printBoard(board, boardSize, players);
			//printCapabilities(players);
			break;
		case 2:
			printf("4: Near Attack.\n");
			printf("5: Distant Attack.\n");
			if((players[i].smartness + players[i].magic) > 150){
				printf("6: Magic Attack.\n");
			}
			scanf("%d", &ansAttack);

			switch(ansAttack){
			case 4:
				playerToAttackNear(players, boardSize, players[i].row, players[i].column, i);
				break;
			case 5:
				for(c=0; c<boardSize; c++){
					for(j=0; j<boardSize; j++){
						foundSlots[i][j].no_of_players = 0;
						for(k=0; k<num_players; k++){
							playersInRange[k] = -1;
						}
					}
				}
				findSlots(boardSize, players, 0, 0, players[i].row, players[i].column, 2, i);
				count = 1;
				break;
			case 6:
				while((ansMagicAttack < 1)||(ansMagicAttack > num_players)){
					printf("Enter the player number you wish to attack.\n");
					scanf("%d", &ansMagicAttack);
				}
				break;
			}
			break;

		case 3:
			//TODO Code if someone leaves the game.
			break;
		}
		printBoard(board, boardSize, players);

	}
	/*	Print the board and player details for the final results.  */
	printf("Final Board:\n");
	printBoard(board, boardSize, players);
	//printFinalResults(players, slots, num_players);1
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
		/*if(board[row][column].up->player3 != 0){	//If a third player is in the slot above then add the players position to the player4 variable.
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){	//If a second player is in the slot above then add the players position to the player3 variable.
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){	//If a player is in the slot above then add the players position to the player2 variable.
			board[row][column].up->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].up->player = current_player+1;
		}*/
		board[row][column].up->player_nos[current_player] = current_player+1;
		board[row][column].up->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row-1;	//set the new value of the players row variable.
		movementType(players, current_player);
		break;

	case 2:
		/*if(board[row][column].right->player3 != 0){	//If a third player is in the slot above then add the players position to the player4 variable.
			board[row][column].right->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){	//If a second player is in the slot above then add the players position to the player3 variable.
			board[row][column].right->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){	//If a player is in the slot above then add the players position to the player2 variable.
			board[row][column].right->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].right->player = current_player+1;
		}*/
		board[row][column].right->player_nos[current_player] = current_player+1;
		board[row][column].right->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].column = players[current_player].column+1;	//set the new value of the players row variable.
		movementType(players, current_player);
		break;

	case 3:
		/*if(board[row][column].down->player3 != 0){	//If a third player is in the slot below then add the players position to the player4 variable.
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){	//If a second player is in the slot below then add the players position to the player3 variable.
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){	//If a player is in the slot below then add the players position to the player2 variable.
			board[row][column].down->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].down->player = current_player+1;
		}*/
		board[row][column].down->player_nos[current_player] = current_player+1;
		board[row][column].down->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].row = players[current_player].row+1;	//set the new value of the players row variable.
		movementType(players, current_player);
		break;
	case 4:
		/*if(board[row][column].left->player3 != 0){	//If a third player is in the slot to the left of then add the players position to the player4 variable.
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){	//If a second player is in the slot to the left of then add the players position to the player3 variable.
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){	//If a player is in the slot to the left of then add the players position to the player2 variable.
			board[row][column].left->player2 = current_player+1;
		}else{	//If the slot is empty then set the players position to the player variable.
			board[row][column].left->player = current_player+1;
		}*/
		board[row][column].left->player_nos[current_player] = current_player+1;
		board[row][column].left->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;	//Set the slot player position to be 0.
		players[current_player].column = players[current_player].column-1;	//set the new value of the players row variable.
		movementType(players, current_player);
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
		printf("1: Move up.\n");
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
		/*if(board[row][column].up->player3 != 0){
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){
			board[row][column].up->player2 = current_player+1;
		}else{
			board[row][column].up->player = current_player+1;
		}*/
		board[row][column].up->player_nos[current_player] = current_player+1;
		board[row][column].up->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		movementType(players, current_player);
		break;

	case 2:
		/*if(board[row][column].right->player3 != 0){
			board[row][column].right->player4 = current_player+1;
		}else if(board[row][column].right->player2 != 0){
			board[row][column].right->player3 = current_player+1;
		}else if(board[row][column].right->player != 0){
			board[row][column].right->player2 = current_player+1;
		}else{
			board[row][column].right->player = current_player+1;
		}*/
		board[row][column].right->player_nos[current_player] = current_player+1;
		board[row][column].right->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].column = players[current_player].column+1;
		movementType(players, current_player);
		break;

	case 3:
		/*if(board[row][column].down->player3 != 0){
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){
			board[row][column].down->player2 = current_player+1;
		}else{
			board[row][column].down->player = current_player+1;
		}*/
		board[row][column].down->player_nos[current_player] = current_player+1;
		board[row][column].down->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].row = players[current_player].row+1;
		movementType(players, current_player);
		break;
	case 4:
		/*if(board[row][column].left->player3 != 0){
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){
			board[row][column].left->player2 = current_player+1;
		}else{
			board[row][column].left->player = current_player+1;
		}*/
		board[row][column].left->player_nos[current_player] = current_player+1;
		board[row][column].left->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].column = players[current_player].column-1;
		movementType(players, current_player);
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
		/*if(board[row][column].up->player3 != 0){
			board[row][column].up->player4 = current_player+1;
		}else if(board[row][column].up->player2 != 0){
			board[row][column].up->player3 = current_player+1;
		}else if(board[row][column].up->player != 0){
			board[row][column].up->player2 = current_player+1;
		}else{
			board[row][column].up->player = current_player+1;
		}*/
		board[row][column].up->player_nos[current_player] = current_player+1;
		board[row][column].up->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].row = players[current_player].row-1;
		movementType(players, current_player);
		break;

	case 2:
		/*if(board[row][column].right->player3 != 0){
			board[row][column].right->player4 = current_player+1;
		}else if(board[row][column].right->player2 != 0){
			board[row][column].right->player3 = current_player+1;
		}else if(board[row][column].right->player != 0){
			board[row][column].right->player2 = current_player+1;
		}else{
			board[row][column].right->player = current_player+1;
		}*/
		board[row][column].right->player_nos[current_player] = current_player+1;
		board[row][column].right->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;
		//board[row][column].player = 0;
		players[current_player].column = players[current_player].column+1;
		movementType(players, current_player);
		break;

	case 3:
		/*if(board[row][column].down->player3 != 0){
			board[row][column].down->player4 = current_player+1;
		}else if(board[row][column].down->player2 != 0){
			board[row][column].down->player3 = current_player+1;
		}else if(board[row][column].down->player != 0){
			board[row][column].down->player2 = current_player+1;
		}else{
			board[row][column].down->player = current_player+1;
		}*/
		board[row][column].down->player_nos[current_player] = current_player+1;
		board[row][column].down->no_of_players +=1;
		board[row][column].player_nos[current_player] = -1;
		board[row][column].no_of_players -=1;

		//board[row][column].player = 0;
		players[current_player].row = players[current_player].row+1;
		movementType(players, current_player);
		break;
	case 4:
		/*if(board[row][column].left->player3 != 0){
			board[row][column].left->player4 = current_player+1;
		}else if(board[row][column].left->player2 != 0){
			board[row][column].left->player3 = current_player+1;
		}else if(board[row][column].left->player != 0){
			board[row][column].left->player2 = current_player+1;
		}else{
			board[row][column].left->player = current_player+1;
		}*/
		board[row][column].left->player_nos[current_player] = current_player+1;
		board[row][column].left->no_of_players +=1;
		board[row][column].no_of_players -=1;
		board[row][column].player_nos[current_player] = -1;
		//board[row][column].player = 0;
		players[current_player].column = players[current_player].column-1;
		movementType(players, current_player);
		break;
	}

}




/*This function changes the capabilities based on the strength values of the current player and the attacked player.*/
void attackPlayerNear(struct player players[], int attacked_player, int current_player){

	//attacked_player-=1; // Decrement attacked_player by 1.

	/*If the attacked_player strength is less than or equal to 70 then use the formula to change the attacked_player's life capability.
	 * Else if the attacked_player's strength is greater than 70 then use the formula to change the current_player's life capability.*/
	if(players[attacked_player].strength <=70){
		players[attacked_player].life = (players[attacked_player].life - (0.5 * players[attacked_player].strength));
	}else if(players[attacked_player].strength >70){
		players[current_player].life = (players[current_player].life - (0.3 * players[attacked_player].strength));
	}
}

/*This function changes the capabilities based on the Dexterity values of the current player and the attacked player.*/
void attackPlayerDistant(struct player players[], int attacked_player, int current_player){

	//attacked_player-=1; // Decrement attacked_player by 1.

	if(players[attacked_player].dexterity >= players[current_player].dexterity){
		return;
	}else if(players[current_player].dexterity > players[attacked_player].dexterity){
		players[attacked_player].life -= 0.3 * players[current_player].strength;

	}
}


void attackPlayerMagic(struct player players[], int attacked_player, int current_player){

	players[attacked_player].life -= ((0.5 * players[current_player].magic) + (0.2 * players[current_player].smartness));

}

void movementType(struct player players[], int current_player){

	if(strcmp(board[players[current_player].row][players[current_player].column].type_of_slot, "Hill") == 0){
		hillMovement(players, current_player);
	}else if(strcmp(board[players[current_player].row][players[current_player].column].type_of_slot, "City") == 0){
		cityMovement(players, current_player);
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
