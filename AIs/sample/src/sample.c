#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int rand_int, rand_max, turn_number;

    scanf("%d", &rand_int);
    scanf("%d", &rand_max);
    scanf("%d", &turn_number);

    fprintf(stderr, "Random value: %d/%d\nTurn number: %d\n", 
            rand_int, rand_max, turn_number);

    srand(rand_int);

    while (!feof(stdin)) {
        int y;
        for (y = 0; y < 5; ++y) {
            int x;
            for (x = 0; x < 5; ++x) {
                int health;
                if (!scanf("%d", &health))
                    return 0;
                fprintf(stderr, "%d ", health);
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
        
        // Make a random move
        printf("%d", rand()%6);
    }

    return 0;
}
