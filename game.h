/*
 * game.h
 *
 *  Created on: 10 Mar 2017
 *      Author: Declan
 */

#ifndef GAME_H_
#define GAME_H_

struct slot **board;

int getBoardSize();
void createBoard(int boardSize);
void printBoard(struct slot **board, int boardSize, struct player players[]);
int userNumberPlayers();
void playersToSlots(int boardSize, struct player players[]);
void runGame(struct player players[], struct slot slots[], int num_players, int num_slots, int boardSize);
void movePlayers(struct slot **board, struct player players[], int boardSize, int current_player);
void twoDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position);
void threeDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position);
void fourDirections(struct slot **board, struct player players[], int boardSize, int current_player);
void attackPlayer(struct player players[], int attacked_player, int current_player);
void hillMovement(struct player players[], int current_player);
void cityMovement(struct player players[], int current_player);
void printFinalResults(struct player players[], struct slot slots[], int num_players);

#endif /* GAME_H_ */
