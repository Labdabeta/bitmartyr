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

#define MAX_AI_COUNT 7
#define MAX_HEALTH 5

static const char *team_names[MAX_AI_COUNT] = {
    "Red", "Green", "Blue", "Grey", "Cyan", "Magenta", "Yellow"
};

// Channel values for a unit at 1 health
static Uint8 one_team_red[MAX_AI_COUNT + 1] = { 
    0x00, 0x33, 0x00, 0x00, 0x33, 0x00, 0x33, 0x33
};
static Uint8 one_team_green[MAX_AI_COUNT + 1] = { 
    0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x00, 0x33
};
static Uint8 one_team_blue[MAX_AI_COUNT + 1] = {
    0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x00
};

void sigpipe_ignore_callback_handler(int signum) {
    printf("Caught SIGPIPE %d\n", signum);
}

void save_screenshot(const char *fname, SDL_Window *wnd, SDL_Renderer *ren)
{
    SDL_Surface *save = NULL;
    SDL_Surface *info = NULL;
    unsigned char *pixbuf = NULL;

    info = SDL_GetWindowSurface(wnd);
    pixbuf = malloc(info->w * info->h * info->format->BytesPerPixel);
    SDL_RenderReadPixels(ren, &info->clip_rect, info->format->format, pixbuf, 
            info->w * info->format->BytesPerPixel);
    save = SDL_CreateRGBSurfaceFrom(pixbuf, 
            info->w, info->h, info->format->BitsPerPixel, 
            info->w * info->format->BytesPerPixel, 
            info->format->Rmask, 
            info->format->Gmask, 
            info->format->Bmask, 
            info->format->Amask);
    SDL_SaveBMP(save, fname);
    SDL_FreeSurface(save);
    free(pixbuf);
    SDL_FreeSurface(info);
}

void print_help(const char *exename) 
{
    printf("%s [width] [height] [scale] [population] [duration] (AI)+ [-- outfile]\n", exename);
    printf("\t [width] \t- The width of the play area\n");
    printf("\t [height] \t- The height of the play area\n");
    printf("\t [scale] \t- The renderer scaling factor\n");
    printf("\t [population] \t- The initial team populations\n");
    printf("\t [duration] \t- The duration of the game, in turns\n");
    printf("\t (AI) \t- The list of AIs, optionally prefixed with a hyphen to indicate\n");
    printf("\t\t\ta basic format AI\n");
    printf("\t [outfile] \t- After a -- an output filename may appear to save each frame in\n");
    printf("\n");
}

void run_game(int width, int height, int scale, int init_pop, int duration, 
              int num_ais, const char *ais[], int is_basic[], 
              const char *outname);

int main(int argc, char *argv[]) 
{
    int i, width, height, scale, init_pop, duration, num_ais;
    const char *ais[MAX_AI_COUNT] = {0};
    int is_basic[MAX_AI_COUNT] = {0};
    const char *outname = NULL;

    // Parse command-line options
    if (argc < 7) {
        print_help(argv[0]);
        return 0;
    }

    width = atoi(argv[1]);
    height = atoi(argv[2]);
    scale = atoi(argv[3]);
    init_pop = atoi(argv[4]);
    duration = atoi(argv[5]);

    for (i = 6; i < argc; ++i) {
        if (!strcmp(argv[i],"--")) {
            if (argc < i + 2) {
                print_help(argv[0]);
                return 0;
            }

            outname = argv[i+1];
            break;
        }
        if (*argv[i] == '-') is_basic[i - 6] = 1;
        ais[i - 6] = argv[i];
        num_ais = i - 5;
    }

    run_game(width, 
            height, 
            scale, 
            init_pop, 
            duration, 
            num_ais, 
            ais, 
            is_basic, 
            outname);

    return 0;
}

void init_c_systems(void)
{
    // Seed the random generator
    srand(time(NULL));
}


void init_sdl_systems(SDL_Window **wnd, SDL_Renderer **ren, 
        int width, int height, int scale)
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return;
    }

    *wnd = SDL_CreateWindow("BitMartyr",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            scale * width, scale * height, SDL_WINDOW_SHOWN);

    if (!(*wnd)) {
        printf("Could not create window: %s\n", SDL_GetError());
        return;
    }

    *ren = SDL_CreateRenderer(*wnd, -1, SDL_RENDERER_ACCELERATED);

    if (!(*ren)) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderSetScale(*ren, scale, scale);
}

struct Unit {
    int health;
    int team;
};

struct GameState {
    struct Unit *units;
    struct Unit *next_units; // used internally by step
    int width, height, num_teams;
    FILE *logfile;
};

struct GameState *init_game_systems(int width, int height, int init_pop, int num_ais);

void step_game_systems(struct GameState *state, const char *ais[], int is_basic[], int turn_number);

// Returns non-0 if the user exitted
int render_game_state(SDL_Window *wnd, SDL_Renderer *ren, struct GameState *state,
        const char *outname, int turn_number);

void close_game_systems(struct GameState *state)
{
    int i;
    unsigned int scores[MAX_AI_COUNT] = {0};
    int max_team = 0;

    for (i = 0; i < state->width * state->height; ++i) {
        if (state->units[i].team) 
            scores[state->units[i].team - 1]++;
    }

    for (i = 0; i < MAX_AI_COUNT; ++i) {
        printf("%s had %d units.\n", team_names[i], scores[i]);
        if (scores[i] > scores[max_team]) 
            max_team = i;
    }

    printf("%s won!\n", team_names[max_team]);

    free(state->units);
    free(state->next_units);
    fclose(state->logfile);
    free(state);
}
    
void close_sdl_systems(SDL_Window *wnd, SDL_Renderer *ren)
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(wnd);
    SDL_Quit();
}

void run_game(int width, int height, int scale, int init_pop, int duration, 
              int num_ais, const char *ais[], int is_basic[], 
              const char *outname)
{
    SDL_Window *wnd;
    SDL_Renderer *ren;
    struct GameState *state;
    int turn_number;

    // Set up the basic C systems
    init_c_systems();

    // Set up the SDL systems
    init_sdl_systems(&wnd, &ren, width, height, scale);

    // Set up game systems
    state = init_game_systems(width, height, init_pop, num_ais);

    // Render turn 0
    if (render_game_state(wnd, ren, state, outname, 0))
        goto cleanup; // User quit the game

    // Run the game
    for (turn_number = 0; turn_number < duration; ++turn_number) {
        step_game_systems(state, ais, is_basic, turn_number);
        if (render_game_state(wnd, ren, state, outname, turn_number + 1))
            goto cleanup; // User quit the game
    }
cleanup:

    // Clean up SDL systems
    close_sdl_systems(wnd, ren);

    // Clean up game systems, and print winner, etc
    close_game_systems(state);
}

struct GameState *init_game_systems(int width, int height, int init_pop, int num_ais)
{
    time_t rawtime;
    struct tm *timeinfo;
    char fname[80];
    int i;
    struct GameState *ret = malloc(sizeof(struct GameState));
    if (!ret) {
        printf("Ran out of memory :'(\n");
        return NULL;
    }

    ret->units = malloc(sizeof(struct Unit) * width * height);
    if (!ret->units) {
        free(ret);
        printf("Ran out of memory :'(\n");
        return NULL;
    }

    ret->next_units = malloc(sizeof(struct Unit) * width * height);
    if (!ret->next_units) {
        free(ret->units);
        free(ret);
        printf("Ran out of memory :'(\n");
        return NULL;
    }

    ret->width = width;
    ret->height = height;
    ret->num_teams = num_ais;

    // Clear out the units
    for (i = 0; i < width * height; ++i) {
        ret->units[i].health = 0;
        ret->units[i].team = 0;
    }

    // Spawn initial locations (TODO: make them equidistant)
    for (i = 0; i < init_pop; ++i) {
        int team_number;
        for (team_number = 0; team_number < num_ais; ++team_number) {
            int spawn_x, spawn_y;
            spawn_x = rand() % width;
            spawn_y = rand() % height;

            ret->units[spawn_y * width + spawn_x].health = 5;
            ret->units[spawn_y * width + spawn_x].team = team_number + 1;
        }
    }

    // Load logfile
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(fname, 80, "%Y_%m_%d_%H_%M_%S.log", timeinfo);
    ret->logfile = fopen(fname, "w");

    // Not a fatal error
    if (!ret->logfile) {
        printf("Can't open log file.");
    }

    return ret;
}

void clear_next_units(struct GameState *state);
void step_game_team(struct GameState *state, int team, const char *ai, int is_basic, int turn_number);
void apply_next_units(struct GameState *state);

void step_game_systems(struct GameState *state, const char *ais[], int is_basic[], int turn_number)
{
    int team_number;

    // Log current turn
    fprintf(state->logfile, "Starting turn %d...\n", turn_number);
    fflush(state->logfile);
     
    // Clear out next units in preparation
    clear_next_units(state);

    for (team_number = 0; team_number < state->num_teams; ++team_number) {
        fprintf(state->logfile, "Running %s ", ais[team_number]);
        fflush(state->logfile);

        step_game_team(state, team_number + 1, ais[team_number], is_basic[team_number], turn_number);

        fprintf(state->logfile, "done.\n");
        fflush(state->logfile);
    }

    // Apply the results and print current status to log file
    apply_next_units(state);
}

// Returns non-0 if the user exitted
int render_game_state(SDL_Window *wnd, SDL_Renderer *ren, struct GameState *state,
        const char *outname, int turn_number)
{
    // Event
    SDL_Event e;

    // Don't try to draw if we couldn't set up a drawing system
    if (wnd && ren) {
        int y;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ren);

        for (y = 0; y < state->height; ++y) {
            int x;
            for (x = 0; x < state->width; ++x) {
                struct Unit u = state->units[y * state->width + x];
                SDL_SetRenderDrawColor(ren, 
                        one_team_red[u.team] * u.health,
                        one_team_green[u.team] * u.health,
                        one_team_blue[u.team] * u.health,
                        SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(ren, x, y);
            }
        }

        SDL_RenderPresent(ren);

        if (outname) {
            char *fname = malloc(strlen(outname) + (turn_number / 10) + 5);
            sprintf(fname, "%s%d.bmp", outname, turn_number);

            save_screenshot(fname, wnd, ren);
            free(fname);
        }
    }

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) 
            return 1;
    }

    return 0;
}

void clear_next_units(struct GameState *state)
{
    int i;
    for (i = 0; i < state->width * state->height; ++i) {
        state->next_units[i].health = 0;
        state->next_units[i].team = 0;
    }
}

void apply_next_units(struct GameState *state)
{
    int i;
    unsigned int scores[MAX_AI_COUNT] = {0};
    unsigned int partial_scores[MAX_HEALTH][MAX_AI_COUNT] = {0};

    for (i = 0; i < state->width * state->height; ++i) {
        state->units[i].team = state->next_units[i].team;
        if (state->units[i].team) {
            state->units[i].health = state->next_units[i].health;
            scores[state->units[i].team]++;
            partial_scores[state->units[i].health-1][state->units[i].team]++;
        } else {
            state->units[i].health = 0;
        }
    }

    for (i = 0; i < MAX_AI_COUNT; ++i) {
        int health;
        fprintf(state->logfile, "Team %s: %d\n", team_names[i], scores[i]);
        fflush(state->logfile);

        for (health = 0; health < MAX_HEALTH; ++health) {
            fprintf(state->logfile, "\t%d-health units: %d\n", 
                    health+1, partial_scores[health][i]);
            fflush(state->logfile);
        }
    }
}

int get_unit_relationship(struct GameState *state, int x, int y, int team)
{
    struct Unit u;
    while (x < 0) x += state->width; 
    while (x > state->width) x -= state->width;
    while (y < 0) y += state->height;
    while (y > state->height) y -= state->height;

    u = state->units[y * state->width + x];

    if (u.team == team) 
        return u.health;
    else
        return -u.health;
}

// Returns -1 if no valid command found before EOF
int read_command(FILE *f)
{
    while (!feof(f)) {
        int ch = fgetc(f);

        switch (ch) {
            case '*': case 'x': case 'X': case '5': return 5;
            case '.': case 's': case 'S': case '0': return 0;
            case '^': case 'u': case 'U': case '1': return 1;
            case '>': case 'r': case 'R': case '2': return 2;
            case 'v': case 'd': case 'D': case '3': return 3;
            case '<': case 'l': case 'L': case '4': return 4;
        }
    }

    return -1;
}

// Applies the given action on the unit at the given coordinate
void apply_action(struct GameState *state, int x, int y, int dir)
{
    int newx, newy, index, newindex;
    newx = x;
    newy = y;
    switch (dir) {
        case 0: break;
        case 1: newy--; break;
        case 2: newx++; break;
        case 3: newy++; break;
        case 4: newx--; break;
        case 5: // suicide time
            return;
    }


    if (newx < 0) newx += state->width;
    if (newx > state->width) newx -= state->width;
    if (newy < 0) newy += state->height;
    if (newy > state->height) newy -= state->height;

    index = y * state->width + x;
    newindex = newy * state->width + newx;

    if (state->units[index].health > state->next_units[newindex].health)
        state->next_units[newindex] = state->units[index];
    else if (state->next_units[newindex].health == state->units[index].health) 
        state->next_units[newindex].team = 0; // Keep old health for a 3rd party

    if (state->next_units[index].health < state->units[index].health - 1) {
        state->next_units[index].health = state->units[index].health - 1;
        state->next_units[index].team = state->units[index].team;
    } else if (state->next_units[index].health == state->units[index].health - 1) {
        // Keep the old health so a 3rd party could win it
        state->next_units[index].team = 0; // Keep old health for a 3rd party
    }
}

void step_game_team(struct GameState *state, int team_number, const char *ai, int is_basic, int turn_number)
{
    int x,y;
    if (is_basic) {
        // In for the long haul
        for (y = 0; y < state->height; ++y) {
            for (x = 0; x < state->width; ++x) {
                int index = y * state->width + x;
                if (state->units[index].team == team_number) {
                    int dx, dy, retval, dir;
                    FILE *tmp, *p;
                    char *command_str;

                    // Generate a temp file
                    char *tmpname = tmpnam(NULL);

                    // Run the AI on it
                    command_str = malloc(
                            strlen(ai) + strlen(" > ") + strlen(tmpname) + 1);
                    sprintf(command_str, "%s > %s", ai, tmpname);
                    p = popen(command_str, "w");

                    fprintf(p, "%d ", rand());
                    fprintf(p, "%d ", RAND_MAX);
                    fprintf(p, "%d ", turn_number);

                    for (dy = -2; dy < 3; ++dy)
                        for (dx = -2; dx < 3; ++dx)
                            fprintf(p, "%d ", get_unit_relationship(state, x + dx, y + dy, team_number));

                    retval = WEXITSTATUS(pclose(p));
                                    
                    // Read input
                    tmp = fopen(tmpname, "r");
                    dir = read_command(tmp);
                    fclose(tmp);
                    remove(tmpname);

                    // Return value used instead of output
                    if (dir < 0) dir = retval;

                    apply_action(state, x, y, dir);
                }
            }
        }
    } else {
        FILE *tmp, *p;
        char *command_str;

        // Generate a temp file
        char *tmpname = tmpnam(NULL);

        // Run the AI on it
        command_str = malloc(strlen(ai) + strlen(" > ") + strlen(tmpname) + 1);
        sprintf(command_str, "%s > %s", ai, tmpname);
        p = popen(command_str, "w");

        fprintf(p, "%d", rand());
        fprintf(p, " %d", RAND_MAX);
        fprintf(p, " %d", turn_number);

        for (y = 0; y < state->height; ++y) {
            for (x = 0; x < state->width; ++x) {
                int index = y * state->width + x;
                if (state->units[index].team == team_number) {
                    int dx, dy, dir;

                    for (dy = -2; dy < 3; ++dy)
                        for (dx = -2; dx < 3; ++dx)
                            fprintf(p, " %d", get_unit_relationship(state, x + dx, y + dy, team_number));
                }
            }
        }

        pclose(p);
        tmp = fopen(tmpname, "r");
        // Now read input
        for (y = 0; y < state->height; ++y) {
            for (x = 0; x < state->width; ++x) {
                int index = y * state->width + x;
                if (state->units[index].team == team_number)
                    apply_action(state, x, y, read_command(tmp));
            }
        }

        fclose(tmp);
        remove(tmpname);
    }
}
