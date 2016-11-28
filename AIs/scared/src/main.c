#include "bitmartyr.h"

Action main(int turn) 
{
    // Danger Levels
    int up, down, left, right;
    int best;
    Action best_dir[4] = {0};
    int num_best;

    up = get_unit(-2, -2).health +
         get_unit(-1, -2).health +
         get_unit( 0, -2).health +
         get_unit( 1, -2).health +
         get_unit( 2, -2).health +
         get_unit(-2, -1).health +
         get_unit(-1, -1).health +
         get_unit( 0, -1).health +
         get_unit( 1, -1).health +
         get_unit( 2, -1).health;

    down = get_unit(-2, 2).health +
           get_unit(-1, 2).health +
           get_unit( 0, 2).health +
           get_unit( 1, 2).health +
           get_unit( 2, 2).health +
           get_unit(-2, 1).health +
           get_unit(-1, 1).health +
           get_unit( 0, 1).health +
           get_unit( 1, 1).health +
           get_unit( 2, 1).health;

    left = get_unit( -2,-2).health +
           get_unit( -2,-1).health +
           get_unit( -2, 0).health +
           get_unit( -2, 1).health +
           get_unit( -2, 2).health +
           get_unit( -1,-2).health +
           get_unit( -1,-1).health +
           get_unit( -1, 0).health +
           get_unit( -1, 1).health +
           get_unit( -1, 2).health;

    right = get_unit( 2,-2).health +
            get_unit( 2,-1).health +
            get_unit( 2, 0).health +
            get_unit( 2, 1).health +
            get_unit( 2, 2).health +
            get_unit( 1,-2).health +
            get_unit( 1,-1).health +
            get_unit( 1, 0).health +
            get_unit( 1, 1).health +
            get_unit( 1, 2).health;

    best = up;
    best_dir[0] = UP;
    num_best = 1;   

    if (down < best) {
        best = down;
        best_dir[0] = DOWN;
        num_best = 1;
    } else if (down == best) {
        best_dir[num_best++] = DOWN;
    }

    if (left < best) {
        best = left;
        best_dir[0] = LEFT;
        num_best = 1;
    } else if (left == best) {
        best_dir[num_best++] = LEFT;
    }

    if (right < best) {
        best = right;
        best_dir[0] = RIGHT;
        num_best = 1;
    } else if (right == best) {
        best_dir[num_best++] = RIGHT;
    }

    return best_dir[uniform_random(0, num_best - 1)];
}
