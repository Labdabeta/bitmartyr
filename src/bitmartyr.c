#include "bitmartyr.h"

#include <stdlib.h>

struct Unit { int health, team; };
struct GameState {
    struct Unit *units, *next_units;

    int width, height, num_teams;
};

static void spawn_units(GameState *game, int pop)
{
    int i;

    // Spawn locations (TODO: make them equidistant)
    for (i = 0; i < pop; ++i) {
        int team_number;
        for (team_number = 0; team_number < game->num_teams; ++team_number) {
            int spawn_index;
            do {
                spawn_index = rand() % (game->width * game->height);
            } while (game->units[spawn_index].team >= 0);

            game->units[spawn_index].health = 5;
            game->units[spawn_index].team = team_number;
        }
    }
}

GameState *create_game(int width, int height, int num_teams, int pop)
{
    int i;
    GameState *ret = malloc(sizeof(GameState));

    ret->units = malloc(sizeof(struct Unit) * width * height);
    ret->next_units = malloc(sizeof(struct Unit) * width * height);

    ret->width = width;
    ret->height = height;
    ret->num_teams = num_teams;

    // Clear out the units
    for (i = 0; i < width * height; ++i) {
        ret->units[i].health = 0;
        ret->units[i].team = -1;
    }

    // Spawn the units
    spawn_units(ret, pop);
    return ret;
}

void free_game(GameState *game)
{
    if (game) {
        free(game->units);
        free(game->next_units);
    }
    free(game);
}

int get_num_teams(const GameState *game) { return game->num_teams; }
int get_width(const GameState *game) { return game->width; }
int get_height(const GameState *game) { return game->height; }
int get_health(const GameState *game, int x, int y) { return game->units[y * game->width + x].health; }
int get_team(const GameState *game, int x, int y) { return game->units[y * game->width + x].team; }

int get_num_units(const GameState *game, int team)
{
    int i;
    int ret = 0;

    for (i = 0; i < game->width * game->height; ++i)
        if (game->units[i].team == team) ret++;

    return ret;
}

static void clear_next_units(GameState *game)
{
    int i;
    for (i = 0; i < game->width * game->height; ++i) {
        game->next_units[i].health = 0;
        game->next_units[i].team = -1;
    }
}

static void load_environment(GameState *game, Environment e, int x, int y)
{
    int ei,dy,dx,team;

    team = game->units[y * game->width + x].team;
    ei = 0;
    for (dy = -2; dy < 3; ++dy) {
        for (dx = -2; dx < 3; ++dx) {
            int nx, ny;
            nx = x+dx; ny = y+dy;

            while (nx < 0) nx += game->width;
            while (nx >= game->width) nx -= game->width;
            while (ny < 0) ny += game->width;
            while (ny >= game->width) ny -= game->width;

            e[ei] = game->units[ny * game->width + nx].health;
            if (game->units[ny * game->width + nx].team != team) e[ei] *= -1;
            ei++;
        }
    }
}

static void apply_action(GameState *game, int x, int y, Action a)
{
    int newx, newy, index, newindex;
    newx = x;
    newy = y;

    switch (a) {
        case STAY: break;
        case UP: newy--; break;
        case RIGHT: newx++; break;
        case DOWN: newy++; break;
        case LEFT: newx--; break;
        case DIE: return;
    }

    if (newx < 0) newx += game->width;
    if (newx >= game->width) newx -= game->width;
    if (newy < 0) newy += game->height;
    if (newy >= game->height) newy -= game->height;

    index = y * game->width + x;
    newindex = newy * game->width + newx;

    if (game->units[index].health > game->next_units[newindex].health)
        game->next_units[newindex] = game->units[index];
    else if (game->units[index].health == game->next_units[newindex].health)
        game->next_units[newindex].team = -1; // Keep old health for a 3rd party

    if (game->next_units[index].health < game->units[index].health - 1) {
        game->next_units[index].health = game->units[index].health - 1;
        game->next_units[index].team = game->units[index].team;
    } else if (game->next_units[index].health == game->units[index].health - 1)
        game->next_units[index].team = -1;
}
        

static void apply_ai(GameState *game, AI_Callback ai, void **data, int x, int y)
{
    int team;
    Environment e;

    team = game->units[y * game->width + x].team;

    if (team >= 0) {
        Action a;
        load_environment(game, e, x, y);
        a = ai(e, data[team]);
        apply_action(game, x, y, a);
    }
}

static void apply_next_units(GameState *game)
{
    int i;
    
    for (i = 0; i < game->width * game->height; ++i) {
        game->units[i].team = game->next_units[i].team;
        game->units[i].health = game->next_units[i].health * (game->units[i].team >= 0);
    }
}

void step(GameState *game, AI_Callback ai, void **data)
{
    int x,y;
    
    clear_next_units(game);
    
    for (y = 0; y < game->height; ++y) {
        for (x = 0; x < game->height; ++x) {
            apply_ai(game, ai, data, x, y);
        }
    }

    apply_next_units(game);          
}
