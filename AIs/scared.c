#include <stdio.h>

#define GET scanf("%d",&tmp)
#define VAL ABS(tmp)

int ABS(int x) { if (x < 0) return -x; return x; }

int main(void)
{
    int tmp;
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
    int best = 0;
    int best_dir[4] = {0};
    int num_best = 0;

    // ignore health and position
    GET; GET; GET;

    // Row 1
    GET; up += VAL; left += VAL;
    GET; up += VAL; left += VAL;
    GET; up += VAL;
    GET; up += VAL; right += VAL;
    GET; up += VAL; right += VAL;

    // Row 2
    GET; up += VAL; left += VAL;
    GET; up += VAL; left += VAL;
    GET; up += VAL;
    GET; up += VAL; right += VAL;
    GET; up += VAL; right += VAL;

    // Row 3
    GET; left += VAL;
    GET; left += VAL;
    GET; right += VAL;
    GET; right += VAL;

    // Row 4
    GET; down += VAL; left += VAL;
    GET; down += VAL; left += VAL;
    GET; down += VAL;
    GET; down += VAL; right += VAL;
    GET; down += VAL; right += VAL;

    // Row 5
    GET; down += VAL; left += VAL;
    GET; down += VAL; left += VAL;
    GET; down += VAL;
    GET; down += VAL; right += VAL;
    GET; down += VAL; right += VAL;

    GET;

    best = up;
    best_dir[0] = 1;
    num_best = 1;

    if (down < best) {
        best = down;
        best_dir[0] = 3;
        num_best = 1;
    } else if (down == best) {
        best_dir[num_best++] = 3;
    }

    if (left < best) {
        best = left;
        best_dir[0] = 4;
        num_best = 1;
    } else if (left == best) {
        best_dir[num_best++] = 4;
    }

    if (right < best) {
        best = right;
        best_dir[0] = 2;
        num_best = 1;
    } else if (right == best) {
        best_dir[num_best++] = 2;
    }

    return best_dir[VAL % num_best];
}
