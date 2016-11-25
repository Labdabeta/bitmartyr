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

typedef enum Direction { UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4 } Direction;
typedef struct Coordinate { int x,y; } Coordinate;
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

#define main(health, pos) bitmartyr_main(health, pos)

#endif /* BITMARTYR_H */
