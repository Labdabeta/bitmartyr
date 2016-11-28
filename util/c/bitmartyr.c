#include "bitmartyr.h"

#include <stdio.h>
#include <stdlib.h>

extern Action bitmartyr_main(Health);

#undef main

/* 
 * 00 01 02 03 04
 * 05 06 07 08 09
 * 10 11 12 13 14
 * 15 16 17 18 19
 * 20 21 22 23 24
 */
static Unit units[25];

int main(int argc, char *argv[])
{
    int rand_int;
    int i;

    scanf("%d", &rand_int);

    srand(rand_int);

    while (!feof(stdin)) {
        for (i = 0; i < 25; ++i) {
            int val;
            scanf("%d",&val);

            if (val > 0) {
                units[i].health = val;
                units[i].allegiance = FRIEND;
            } else if (val < 0) {
                units[i].health = -val;
                units[i].allegiance = ENEMY;
            } else {
                units[i].allegiance = NONE;
            }
        }
        units[12].allegiance = SELF;

        printf("%d",bitmartyr_main(units[12].health));
    }

    return 0;
}

int uniform_random(int min, int max)
{
    double r = rand() / (1.0 + RAND_MAX);
    int range = max - min + 1;
    return min + (r * range);
}

Unit get_unit(int dx, int dy)
{
    return units[(dy + 2) * 5 + (dx + 2)];
}

int get_relationship(int dx, int dy)
{
    Unit u = get_unit(dx, dy);
    if (u.allegiance == FRIEND || u.allegiance == SELF) 
        return u.health;
    if (u.allegiance == ENEMY)
        return -u.health;
    return 0;
}
