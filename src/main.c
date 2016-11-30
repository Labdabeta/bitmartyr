#include "bitmartyr.h"
#include "bitmartyr_gui.h"
#include "sleep.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <poll.h>

#define MAX_AI_COUNT 7
#define TIMEOUT_MS 1000

static const char *team_names[MAX_AI_COUNT] = {
    "Red", "Green", "Blue", "Grey", "Cyan", "Magenta", "Yellow"
};

// Channel values for a unit at 1 health
static int one_team_red[MAX_AI_COUNT] = { 
    0x33, 0x00, 0x00, 0x33, 0x00, 0x33, 0x33
};
static int one_team_green[MAX_AI_COUNT] = { 
    0x00, 0x33, 0x00, 0x33, 0x33, 0x00, 0x33
};
static int one_team_blue[MAX_AI_COUNT] = {
    0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x00
};

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
    srand(time(NULL));

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
        if (*argv[i] == '-') {
            is_basic[i - 6] = 1;
            ais[i - 6] = argv[i]+1;
        } else {
            is_basic[i - 6] = 0;
            ais[i - 6] = argv[i];
        }
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

struct AI_Data {
    /* These will both be NULL in basic format AIs */
    FILE *results;
    FILE *pipe;

    char *fname;
};

Action load_ai(Environment e, void *fd)
{
    struct AI_Data *data = fd;

    if (data->results) {
        /* Standard format. */
        int i;
        struct pollfd pollvar;
        for (i = 0; i < 25; ++i) 
            fprintf(data->pipe, "%d ", e[i]);

        fflush(data->pipe);

        pollvar.fd = fileno(data->results);
        pollvar.events = POLLIN;
        while (poll(&pollvar, 1, TIMEOUT_MS) > 0) {
            clearerr(data->results);
            char ch = fgetc(data->results);
            
            if (ch == EOF) {
                clearerr(data->results);
                continue;
            }

            switch (ch) {
                case '*': case 'x': case 'X': case '5': return DIE;
                case '.': case 's': case 'S': case '0': return STAY;
                case '^': case 'u': case 'U': case '1': return UP;
                case '>': case 'r': case 'R': case '2': return RIGHT;
                case 'v': case 'd': case 'D': case '3': return DOWN;
                case '<': case 'l': case 'L': case '4': return LEFT;
            }
        }
            
        return DIE;
    } else {
        /* Basic format. */
        int i,retval;
        FILE *pipe = NULL;
        FILE *tmp = NULL;
        char *tmpname = tmpnam(NULL);
        char *command_str = malloc(strlen(data->fname) + strlen(" > ") + strlen(tmpname) + 1);
        sprintf(command_str, "%s > %s", data->fname, tmpname);
        
        pipe = popen(command_str, "w");
        free(command_str);
        for (i = 0; i < 25; ++i)
            fprintf(pipe, "%d ", e[i]);
        
        retval = WEXITSTATUS(pclose(pipe));

        tmp = fopen(tmpname, "r");
        while (!feof(tmp)) {
            char ch = fgetc(tmp);

            switch (ch) {
                case '*': case 'x': case 'X': case '5': 
                    fclose(tmp); remove(tmpname);
                    return DIE;
                case '.': case 's': case 'S': case '0': 
                    fclose(tmp); remove(tmpname);
                    return STAY;
                case '^': case 'u': case 'U': case '1': 
                    fclose(tmp); remove(tmpname);
                    return UP;
                case '>': case 'r': case 'R': case '2': 
                    fclose(tmp); remove(tmpname);
                    return RIGHT;
                case 'v': case 'd': case 'D': case '3': 
                    fclose(tmp); remove(tmpname);
                    return DOWN;
                case '<': case 'l': case 'L': case '4': 
                    fclose(tmp); remove(tmpname);
                    return LEFT;
            }
        }

        fclose(tmp); remove(tmpname);
        return retval;
    }
}

struct AI_Data **load_ai_data(int num_ais, const char *ais[], int is_basic[])
{
    int i;
    struct AI_Data **ret = malloc(sizeof(struct AI_Data *) * num_ais);
    for (i = 0; i < num_ais; ++i) {
        ret[i] = malloc(sizeof(struct AI_Data));
        if (is_basic[i]) {
            ret[i]->results = NULL;
            ret[i]->pipe = NULL;
            ret[i]->fname = (char*)ais[i];
        } else {
            char *tmpname = tmpnam(NULL);
            char *command_str = malloc(strlen(ais[i]) + strlen(" > ") + strlen(tmpname) + 1);
            sprintf(command_str, "%s > %s", ais[i], tmpname);
            ret[i]->pipe = popen(command_str, "w");

            // Wait to be able to get the output file
            while (!(ret[i]->results = fopen(tmpname, "r"))) {
                sleep_ms(100);
            }
            ret[i]->fname = malloc(strlen(tmpname) + 1);
            strcpy(ret[i]->fname, tmpname);
            free(command_str);
        }
    }

    return ret;
}

void free_ai_data(void **data, int num_ais)
{
    int i;
    struct AI_Data **d = (struct AI_Data **)data;

    for (i = 0; i < num_ais; ++i) {
        if (d[i]->results) {
            fclose(d[i]->results);
            remove(d[i]->fname);
            free(d[i]->fname);
        }
        if (d[i]->pipe) fclose(d[i]->pipe);
        free(d[i]);
    }
    free(d);
}

void print_results(GameState *game, int num_ais, const char *ais[])
{
    int i;
    int max = -1;
    int max_team = -1;
    for (i = 0; i < num_ais; ++i) {
        int num_units = get_num_units(game, i);
        printf("%s (%s) had %d units.\n", team_names[i], 
                strrchr(ais[i], '/') + 1, num_units);
        if (num_units > max) {
            max = num_units;
            max_team = i;
        }
    }

    printf("%s (%s) won!\n", team_names[max_team], strrchr(ais[max_team], '/') + 1);
}

void run_game(int width, int height, int scale, int init_pop, int duration, 
              int num_ais, const char *ais[], int is_basic[], 
              const char *outname)
{
    int i;
    GUIContext *gui;
    GameState *game;
    void **data = (void**)load_ai_data(num_ais, ais, is_basic);

    init_gui();
    gui = create_gui_context(width, height, scale);
    game = create_game(width, height, num_ais, init_pop);

    render_game(game, gui, one_team_red, one_team_green, one_team_blue);
    for (i = 0; i < duration && !handle_events(); ++i) {
        step(game, load_ai, data);
        render_game(game, gui, one_team_red, one_team_green, one_team_blue);
        if (outname) {
            char *fname = malloc(strlen(outname) + 20);
            sprintf(fname, "%s%d.bmp", outname, i);
            save_render(gui, fname);
            free(fname);
        }
    }

    print_results(game, num_ais, ais);
    
    free_gui_context(gui);
    free_game(game);
    close_gui();
}

