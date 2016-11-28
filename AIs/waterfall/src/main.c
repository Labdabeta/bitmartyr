#include "bitmartyr.h"

Action main(int turn)
{
    int myhealth = get_unit(0, 0).health;

    switch (myhealth) {
        case 5: return RIGHT;
        case 4: return DOWN;
        case 3: return LEFT;
        case 2: return UP;
        default: return STAY;
    }
}
