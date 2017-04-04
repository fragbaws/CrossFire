/*
 * changingValues.h
 *
 *  Created on: 10 Mar 2017
 *      Author: Declan
 */

#ifndef CHANGINGVALUES_H_
#define CHANGINGVALUES_H_

/*  This statement allows these global variables, declared in main.c, to be accessed by every file where this
 * 	header is included  */
extern int num_players, num_slots;

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
};

/*	Defining an enumerator which includes slot types  */
enum slot_types{
	GROUND,
	HILL,
	CITY,
};

/*	Defining a struct named slots with the needed variables  */
struct slot{
	enum slot_types type;
	int player, player2, player3, player4;
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

#endif /* CHANGINGVALUES_H_ */
