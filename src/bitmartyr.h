#ifndef BITMARTYR_H
#define BITMARTYR_H

typedef struct GameState GameState;

GameState *create_game(int width, int height, int num_teams, int pop);
void free_game(GameState *game);

int get_num_teams(const GameState *game);
int get_width(const GameState *game);
int get_height(const GameState *game);
int get_health(const GameState *game, int x, int y);

/* Teams are 0-indexed. -1 for no team. */
int get_team(const GameState *game, int x, int y);
int get_num_units(const GameState *game, int team);

typedef enum Action { STAY = 0, UP, RIGHT, DOWN, LEFT, DIE } Action;
typedef int Environment[25];

typedef Action (*AI_Callback)(Environment, void *);

/* Each element of data will be passed to the AI for its team. 
 * It is up to the caller to ensure enough data is available for each team. */
void step(GameState *game, AI_Callback ai, void **data);

typedef void (*AI_Turnwise_Callback)(Environment*, Action*, int, void*);

void step_turnwise(GameState *game, AI_Turnwise_Callback ai, void **data);

#endif /* BITMARTYR_H */
