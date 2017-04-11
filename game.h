#ifndef GAME_H_
#define GAME_H_

struct slot **board;
struct slot **foundSlots;

void createBoard(int boardSize);
void printBoard(struct slot **board, int boardSize, struct player players[]);
int userNumberPlayers();
void playersToSlots(int boardSize, struct player players[]);
void runGame(struct player players[], struct slot slots[], int num_players, int num_slots, int boardSize);
void findSlots(int boardSize, struct player players[], int exploredRow, int exploredColumn, int currentRow, int currentColumn, int reqDistance, int current_player);
void playerToAttack(struct player players[], int boardSize, int currentRow, int currentColumn, int current_player);
void playerToAttackNear(struct player players[], int boardSize, int currentRow, int currentColumn, int current_player);
void attackPlayerMagic(struct player players[], int attacked_player, int current_player);
void movementType(struct player players[], int current_player);
void typetoSlot(int boardSize);
void movePlayers(struct slot **board, struct player players[], int boardSize, int current_player);
void twoDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position);
void threeDirections(struct slot **board, struct player players[], int boardSize, int current_player, int position);
void fourDirections(struct slot **board, struct player players[], int boardSize, int current_player);
void attackPlayerNear(struct player players[], int attacked_player, int current_player);
void attackPlayerDistant(struct player players[], int attacked_player, int current_player);
void hillMovement(struct player players[], int current_player);
void cityMovement(struct player players[], int current_player);
void printFinalResults(struct player players[], int num_players);

#endif /* GAME_H_ */
