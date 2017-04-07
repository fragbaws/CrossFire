#ifndef CHANGINGVALUES_H_
#define CHANGINGVALUES_H_

/*  This statement allows these global variables, declared in main.c, to be accessed by every file where this
 *  header is included  */
extern int num_players, num_slots;

/*Defining a struct named players with the needed variables.*/
struct player{
        char type[20];
        char name[10];
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

/*  Defining a struct named slots with the needed variables  */
struct slot{
    char slot_type[7];

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

#endif /* CHANGINGVALUES_H_ */
