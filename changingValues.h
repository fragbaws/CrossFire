#include <stdbool.h>
#ifndef CHANGINGVALUES_H_
#define CHANGINGVALUES_H_

/*  This statement allows these global variables, declared in main.c, to be accessed by every file where this
 * 	header is included  */
extern int num_players, num_slots;
int count;
int playersInRange[6];
int playersInRangeCounter;

/*Defining a struct named players with the needed variables.*/
struct player{
		char type[20];
		char name[50];
		int smartness;
		int strength;
		int dexterity;
		int magic;
		int luck;
		int life;
		int row;
		int column;
		int position;
		int left_game;
};

/*	Defining an enumerator which includes slot types  */
enum slot_types{
	GROUND,
	HILL,
	CITY,
};

/*	Defining a struct named slots with the needed variables  */
struct slot{
	char type_of_slot[7];
	int no_of_players;
	int player_nos[6];
	int row;
	int column;

	struct slot *left;
	struct slot *right;
	struct slot *up;
	struct slot *down;
};


void setElfValues(struct player *name);
void setHumanValues(struct player *name);
void setOgreValues(struct player *name);
void setWizardValues(struct player *name);
void playersFromUser(struct player players[]);
void printCapabilities(struct player players[]);

#endif /* CHANGINGVALUES_H_ */
