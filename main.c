#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define WEXITSTATUS(X) (X)
#define signal(X,Y) 
#else
#include <sys/wait.h>
#include <signal.h>
#endif

void sigpipe_ignore_callback_handler(int signum) {
    printf("Caught SIGPIPE %d\n", signum);
}

#define WIDTH 100
#define HEIGHT 100
#define NUM_TURNS 1000

#define INIT_SIZE 5
#define MAX_HEALTH 5
#define HEALTH_SIZE (255 / MAX_HEALTH)

#define SCALE 10.0

struct Unit {
    int health;
    int team; // 0 = no team, n = argv[n]
};
int main(int argc, char *argv[]) 
{
    SDL_Window *wnd;
    SDL_Renderer *ren;
    SDL_Event e;

    // b is this turn, n is tentative next turn
    struct Unit *b,*n;

    unsigned int scores[3] = {0};
    unsigned int tmp_scores[3] = {0};

    int turn_number, spawn;

    srand(time(NULL));

    // Ignore broken pipes so people can return early
    signal(SIGPIPE, sigpipe_ignore_callback_handler);

    b = calloc(sizeof(struct Unit), WIDTH*HEIGHT);
    n = calloc(sizeof(struct Unit), WIDTH*HEIGHT);

    if (argc != 4) {
        printf("3 program names required.\n");
        return 0;
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    wnd = SDL_CreateWindow("Bitmartyr",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCALE * WIDTH, SCALE * HEIGHT, SDL_WINDOW_SHOWN);

    ren = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(ren, SCALE, SCALE);

    // init
    for (spawn = 0; spawn < INIT_SIZE; ++spawn) {
        int idx, tid;
        for (tid = 1; tid < 4; ++tid) {
            do {
                idx = rand() % (WIDTH * HEIGHT);
            } while (b[idx].team);

            b[idx].team = tid;
            b[idx].health = MAX_HEALTH;
        }
    }

    for (turn_number = 0; turn_number < NUM_TURNS; ++turn_number) {
        int y;

        int wait = 0;

        printf("Starting turn %d...\n", turn_number);
        printf("Red: %u (%u)\tGreen: %u (%u)\tBlue: %u (%u)\n", 
                scores[0], tmp_scores[0],
                scores[1], tmp_scores[1],
                scores[2], tmp_scores[2]);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ren);

        for (y = 0; y < HEIGHT; ++y) {
            size_t x; 
            for (x = 0; x < WIDTH; ++x) {
                if (b[y * WIDTH + x].team) {
                    switch (b[y * WIDTH + x].team) {
                        case 1: SDL_SetRenderDrawColor(ren, HEALTH_SIZE * b[y * WIDTH + x].health, 0, 0, SDL_ALPHA_OPAQUE); break;
                        case 2: SDL_SetRenderDrawColor(ren, 0, HEALTH_SIZE * b[y * WIDTH + x].health, 0, SDL_ALPHA_OPAQUE); break;
                        case 3: SDL_SetRenderDrawColor(ren, 0, 0, HEALTH_SIZE * b[y * WIDTH + x].health, SDL_ALPHA_OPAQUE); break;
                        default: SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
                    }
                    SDL_RenderDrawPoint(ren, x, y);
                }
            }
        }

        SDL_RenderPresent(ren);

        for (y = 0; y < HEIGHT; ++y) {
            int x;
            for (x = 0; x < WIDTH; ++x) {
                if (b[y * WIDTH + x].team) {
                    int dy;
                    int newindex;
                    int index = y * WIDTH + x;
                    int team = b[index].team;
                    FILE *p = popen(argv[team], "w");

                    fprintf(p, "%d %d %d ", b[index].health, x, y);

                    for (dy = -2; dy < 3; ++dy) {
                        int dx;
                        for (dx = -2; dx < 3; ++dx) {
                            if (dx || dy) {
                                int check_index;
                                int check_x = x + dx;
                                int check_y = y + dy;

                                if (check_x < 0) check_x += WIDTH;
                                if (check_y < 0) check_y += HEIGHT;
                                if (check_x > WIDTH) check_x -= WIDTH;
                                if (check_y > HEIGHT) check_y += HEIGHT;

                                check_index = check_y * WIDTH + check_x;

                                // Magic math if unrolling...
                                fprintf(p, "%d ", (-1 * (b[check_index].team == team)) * (!!b[check_index].team) * b[check_index].health);
                            }
                        }
                    }

                    fprintf(p, "%d ", rand());
                    fprintf(p, "%d ", RAND_MAX);

                    fprintf(p, "%d ", turn_number);

                    switch (WEXITSTATUS(pclose(p))) {
                        case 1: newindex = (y-1) * WIDTH + x; break;
                        case 2: newindex = y * WIDTH + x + 1; break;
                        case 3: newindex = (y+1) * WIDTH + x; break;
                        case 4: newindex = y * WIDTH + x - 1; break;
                        default: 
                            // Invalid input, you're dead
                            newindex = -1;
                    }

                    if (newindex >= 0) {
                        newindex %= WIDTH * HEIGHT;
                        if (n[newindex].health < b[index].health) n[newindex] = b[index];
                        
                        // Keep the old health so a 3rd party could win it
                        else if (n[newindex].health == b[index].health) n[newindex].team = 0;
                    }

                    if (n[index].health < b[index].health - 1) {
                        n[index].health = b[index].health - 1;
                        n[index].team = b[index].team;
                    } else if (n[index].health == b[index].health - 1) n[index].team = 0;
                }

                while (SDL_PollEvent(&e)) {
                    switch (e.type) {
                        case SDL_QUIT: goto end;
                        case SDL_KEYDOWN: wait = 0;
                    }
                }
            }
        }

        tmp_scores[0] = tmp_scores[1] = tmp_scores[2] = 0;
        for (y = 0; y < HEIGHT; ++y) {
            size_t x;

            for (x = 0; x < WIDTH; ++x) {
                b[y * WIDTH + x].team = n[y * WIDTH + x].team;
                if (b[y * WIDTH + x].team) {
                    b[y * WIDTH + x].health = n[y * WIDTH + x].health;
                    tmp_scores[b[y * WIDTH + x].team - 1] += b[y * WIDTH + x].health;
                } else {
                    b[y * WIDTH + x].health = 0;
                }

                n[y * WIDTH + x].team = 0;
                n[y * WIDTH + x].health = 0;
            }
        }

        if (tmp_scores[0] >= tmp_scores[1] && tmp_scores[0] >= tmp_scores[2]) {
            scores[0]++;
        } 
        if (tmp_scores[1] >= tmp_scores[0] && tmp_scores[1] >= tmp_scores[2]) {
            scores[1]++;
        }
        if (tmp_scores[2] >= tmp_scores[0] && tmp_scores[2] >= tmp_scores[1]) {
            scores[2]++;
        }

        while (wait) {
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT: goto end;
                    case SDL_KEYDOWN: wait = 0;
                }
            }
        }
    } 

end:
    free(b);
    free(n);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);
    SDL_Quit();

    return 0;
}
