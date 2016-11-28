/*******************************************************************************
 * Author: Louis A. Burke
 *
 * Utilities for bitmartyr AI play.
 *
 * To use this header provide your own main() function which returns a Direction
 * type and takes a health and a coordinate. They represent your current unit.
 ******************************************************************************/
#ifndef BITMARTYR_H
#define BITMARTYR_H

typedef enum Action { STAY = 0, UP = 1, LEFT = 4, DOWN = 3, RIGHT = 2, DIE = 5 } Action;
typedef int Health;
typedef enum Allegiance { NONE = 0, FRIEND, ENEMY, SELF } Allegiance;
typedef struct Unit { Health health; Allegiance allegiance; } Unit;

/* Returns a uniform random integer X such that min <= X <= max */
int uniform_random(int min, int max);

/* Returns the unit at the relative coordinate specified. */
Unit get_unit(int dx, int dy);

/* Returns the positive/negative value of health relative to self at the
 * relative coordinate specified as received on input. */
int get_relationship(int dx, int dy);

#define main(health) bitmartyr_main(health)

#endif /* BITMARTYR_H */
