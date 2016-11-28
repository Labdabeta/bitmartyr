#include "bitmartyr.h"

#include <stdio.h>
#include <stdlib.h>

extern Action bitmartyr_main(int);

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
    int turn_number;
    int i,x,y;

    scanf("%d", &rand_int);
    srand(rand_int);
    
    // Consume rand max
    scanf("%*d");
    scanf("%d", &turn_number);

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

            units[i].up = units[i].left = units[i].down = units[i].right = NULL;
        }
        units[12].allegiance = SELF;

        // Create links
        for (y = -2; y < 3; ++y) {
            for (x = -2; x < 3; ++x) {
                if (y > -2) units[(y+2)*5+x+2].up = &units[(y+1)*5+x+2];
                if (y < 2) units[(y+2)*5+x+2].down = &units[(y+3)*5+x+2];
                if (x > -2) units[(y+2)*5+x+2].left = &units[(y+2)*5+x+1];
                if (x < 2) units[(y+2)*5+x+2].right = &units[(y+2)*5+x+3];
            }
        }

        printf("%d",bitmartyr_main(turn_number));
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
