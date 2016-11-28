/*******************************************************************************
 * Author: Louis A. Burke
 *
 * Utilities for bitmartyr AI play.
 *
 * To use this header provide your own main() function which returns an Action.
 ******************************************************************************/
#ifndef BITMARTYR_H
#define BITMARTYR_H

typedef enum Action { STAY = 0, UP = 1, LEFT = 4, DOWN = 3, RIGHT = 2, DIE = 5 } Action;
typedef enum Allegiance { NONE = 0, FRIEND, ENEMY, SELF } Allegiance;
typedef struct Unit { 
    int health; 
    int relationship; // Raw relationship to your character -ve for enemies
    Allegiance allegiance; 
    struct Unit *up;
    struct Unit *left;
    struct Unit *down;
    struct Unit *right;
} Unit;

/* Returns a uniform random integer X such that min <= X <= max */
int uniform_random(int min, int max);

/* Returns the unit at the relative coordinate specified. */
Unit get_unit(int dx, int dy);

#define main(...) bitmartyr_main(void)

#endif /* BITMARTYR_H */
