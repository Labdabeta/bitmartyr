#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() 
{
    int health, x, y;
    int s[24];
    int i;
    srand(time(NULL));

    scanf("%d", &health);
    scanf("%d", &x);
    scanf("%d", &y);

    for (i = 0; i < 24; ++i) scanf("%d", &s[i]);

    scanf("%d", &i);

    return (i % 4) + 1;
}
    
