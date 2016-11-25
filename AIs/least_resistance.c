#include "bitmartyr.h"

Direction main(Health health, Coordinate pos)
{
    int up_weight, down_weight, left_weight, right_weight;
    int best_weight; Direction best_direction[4]; //up to 4 best directions
    int num_winners;

    // Check the squares above us. 
    up_weight = get_relationship(-1, 0) +
                get_relationship(-2,-1) +
                get_relationship(-2, 0) +
                get_relationship(-2, 1);

    // Check the squares below us.
    down_weight = get_relationship(1, 0) +
                  get_relationship(2,-1) +
                  get_relationship(2, 0) +
                  get_relationship(2, 1);

    // Check the squares to the left of us.
    left_weight = get_relationship( 0,-1) +
                  get_relationship(-1,-2) +
                  get_relationship( 0,-2) +
                  get_relationship( 1,-2);

    // Check the squares to the right of us.
    right_weight = get_relationship( 0,1) +
                   get_relationship(-1,2) +
                   get_relationship( 0,2) +
                   get_relationship( 1,2);

    best_weight = up_weight;
    best_direction[0] = UP;
    num_winners = 1;

    if (down_weight > best_weight) {
        best_weight = down_weight;
        best_direction[0] = DOWN;
        num_winners = 1;
    } else if (down_weight == best_weight) {
        best_direction[num_winners++] = DOWN;
    }

    if (left_weight > best_weight) {
        best_weight = left_weight;
        best_direction[0] = LEFT;
        num_winners = 1;
    } else if (left_weight == best_weight) {
        best_direction[num_winners++] = LEFT;
    }

    if (right_weight > best_weight) {
        best_weight = right_weight;
        best_direction[0] = RIGHT;
        num_winners = 1;
    } else if (right_weight == best_weight) {
        best_direction[num_winners++] = RIGHT;
    }

    return best_direction[uniform_random(0,num_winners - 1)];
}
