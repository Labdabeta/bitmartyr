#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
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
        fflush(0);
    }

    return 0;
}
