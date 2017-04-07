#ifndef GAME_H_
#define GAME_H_

struct slot **board;

int getBoardSize();
void createBoard(int boardSize);
void printBoard(int boardSize, struct player players[]);
int userNumberPlayers();
void typeSlot(int boardSize);
void playersToSlots(int boardSize, struct player players[]);
void runGame(struct player players[], struct slot slots[], int num_players, int num_slots, int boardSize);
void findSlots(int boardSize, struct player players[], int currentRow, int currentColumn, int distance);
void movePlayers(struct player players[], int boardSize, int current_player);
void twoDirections(struct player players[], int boardSize, int current_player, int position);
void threeDirections(struct player players[], int boardSize, int current_player, int position);
void fourDirections(struct player players[], int boardSize, int current_player);
void attackPlayer(struct player players[], int attacked_player, int current_player);
void hillMovement(struct player players[], int current_player);
void cityMovement(struct player players[], int current_player);
void printFinalResults(struct player players[], struct slot slots[], int num_players);

#endif /* GAME_H_ */
