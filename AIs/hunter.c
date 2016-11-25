#include "bitmartyr.h"

Direction main(Health health, Coordinate coord) 
{
    int i,j;
    Direction tmp;
    Direction dirs[4] = { UP, LEFT, DOWN, RIGHT };

    // Generate a direction permutation
    i = uniform_random(0,3); tmp = dirs[3]; dirs[3] = dirs[i]; dirs[i] = tmp;
    i = uniform_random(0,2); tmp = dirs[2]; dirs[2] = dirs[i]; dirs[i] = tmp;
    i = uniform_random(0,1); tmp = dirs[1]; dirs[1] = dirs[i]; dirs[i] = tmp;

    // Check for killable targets
    j = 0;
    for (i = 0; i < 3; ++i) {
        Unit target = {0};
        switch (dirs[i]) {
            case UP: target = get_unit(0, -1); break;
            case DOWN: target = get_unit(0, 1); break;
            case LEFT: target = get_unit(-1, 0); break;
            case RIGHT: target = get_unit(1, 0); break;
        }

        // If the target is a weaker enemy, go there to kill it!
        if (target.allegiance == ENEMY && target.health <= health && target.health > j) {
            j = target.health;
            tmp = dirs[i];
        }
    }
    if (j > 0) return tmp;

    // Check for safe movements
    for (i = 0; i < 3; ++i) {
        Unit target = {0};
        switch (dirs[i]) {
            case UP: target = get_unit(0, -1); break;
            case DOWN: target = get_unit(0, 1); break;
            case LEFT: target = get_unit(-1, 0); break;
            case RIGHT: target = get_unit(1, 0); break;
        }

        if (target.allegiance == NONE) return dirs[i];
    }

    // We have to kill a teammate, kill the weakest
    j = 10000;
    for (i = 0; i < 3; ++i) {
        Unit target = {0};
        switch (dirs[i]) {
            case UP: target = get_unit(0, -1); break;
            case DOWN: target = get_unit(0, 1); break;
            case LEFT: target = get_unit(-1, 0); break;
            case RIGHT: target = get_unit(1, 0); break;
        }

        if (target.allegiance == FRIEND && target.health < j) {
            j = target.health;
            tmp = dirs[i];
        }
    }
    
    // we have to have found somebody!
    return tmp;
}
