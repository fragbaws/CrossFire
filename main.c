#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "changingValues.h"
#include "game.h"
/*  These variables are called as global to make them readily available to all functions without having to declare them in the
	prototypes of a large number of functions  */
int num_players, num_slots;

int main(){

	int boardSize, round=0;
	setbuf(stdout, NULL);

		num_players = userNumberPlayers();
		/*Creating a array named players of size num_players and it is of type struct
		player.*/

		struct player players[num_players];

		/*Call function to generate the list of players*/
		playersFromUser(players);

		for(int i=0;i<num_players;i++)
		{
			players[i].left_game = 0; // initialising the players to be in the game
		}

		/*Create array of size num_slots and to store slots*/
			struct slot slots[num_slots];

			/*get the user to enter the boardSize and store the returned value in the boardSize variable*/
			boardSize = 7;

			/*Create the board and the slots.*/
			createBoard(boardSize);

			/*Assign all of the players to randomly picked slots.*/
			playersToSlots(boardSize, players);

			typetoSlot(boardSize);

	int number = num_players;

	while(num_players >= 1){

		count=1;
		playersInRangeCounter = 0;

		round++;	//Update the round

		printf("\n\n\n");

		//print the current round.
		printf("**************************\n");
		printf("\tROUND %d\n", round);
		printf("**************************\n");


		/*Play the game.*/
		num_players = runGame(players, slots, num_players, num_slots, boardSize);

		/*If there is only one player left in the game then print the congratulations statement.*/
		if(num_players == 1)
		{
			for(int i = 0;i<number;i++)
			{
				if(players[i].left_game != 1 && players[i].life >0)
				{
					printf("\nCongratulations, %s! You are the last player standing!\n", players[i].name);
					printFinalResults(players, i);
					return 0;
				}
			}
		}
	}
	return 0;
}
