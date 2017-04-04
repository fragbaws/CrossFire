/*
 * main.c
 *
 *  Created on: 10 Mar 2017
 *      Author: Declan
 */
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

	int boardSize;
	setbuf(stdout, NULL);

		num_players = userNumberPlayers();
		/*Creating a array named players of size num_players and it is of type struct
		player.*/
		struct player players[num_players];

		/*Call function to generate the list of players*/
		playersFromUser(players);

		/*Create array of size num_slots and to store slots*/
		struct slot slots[num_slots];

		/*get the user to enter the boardSize and store the returned value in the boardSize variable*/
		boardSize = getBoardSize();

		/*Create the board and the slots.*/
		createBoard(boardSize);

		/*Assign all of the players to randomly picked slots.*/
		playersToSlots(boardSize, players);

		/*Play the game.*/
		runGame(players, slots, num_players, num_slots, boardSize);

	return 0;
}


