#ifndef BITMARTYR_GUI_H
#define BITMARTYR_GUI_H

#include "bitmartyr.h"

void init_gui(void);
void close_gui(void);
int handle_events(void); /* Returns non-zero if the user quit. */

typedef struct GUIContext GUIContext;

GUIContext *create_gui_context(int width, int height, int scale);
void free_gui_context(GUIContext *context);

/* Base colours are indexed as per teams, then multiplied by health. */
void render_game(const GameState *game, GUIContext *context, 
                 int *base_red, int *base_green, int *base_blue);

void save_render(GUIContext *context, const char *fname);

#endif /* BITMARTYR_GUI_H */
