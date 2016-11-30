#include "bitmartyr_gui.h"

#include <SDL2/SDL.h>

struct GUIContext {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void init_gui(void) { SDL_Init(SDL_INIT_EVERYTHING); }
void close_gui(void) { SDL_Quit(); }

int handle_events(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return 1;
    }
    return 0;
}

GUIContext *create_gui_context(int width, int height, int scale)
{
    GUIContext *ret = malloc(sizeof(GUIContext));

    ret->window = SDL_CreateWindow("BitMartyr",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            scale * width, scale * height, SDL_WINDOW_SHOWN);
    
    ret->renderer = SDL_CreateRenderer(ret->window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(ret->renderer, scale, scale);

    return ret;
}

void free_gui_context(GUIContext *context)
{
    if (context) {
        SDL_DestroyRenderer(context->renderer);
        SDL_DestroyWindow(context->window);
    } 
    free(context);
}

void render_game(const GameState *game, GUIContext *context,
                 int *base_red, int *base_green, int *base_blue)
{
    int x,y;
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(context->renderer);

    for (y = 0; y < get_height(game); ++y) {
        for (x = 0; x < get_width(game); ++x) {
            SDL_SetRenderDrawColor(context->renderer,
                    base_red[get_team(game,x,y)] * get_health(game,x,y),
                    base_green[get_team(game,x,y)] * get_health(game,x,y),
                    base_blue[get_team(game,x,y)] * get_health(game,x,y),
                    SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(context->renderer, x, y);
        }
    }

    SDL_RenderPresent(context->renderer);
}

void save_render(GUIContext *context, const char *fname)
{
    SDL_Surface *save = NULL;
    SDL_Surface *info = NULL;
    unsigned char *pixbuf = NULL;

    info = SDL_GetWindowSurface(context->window);
    pixbuf = malloc(info->w * info->h * info->format->BytesPerPixel);
    SDL_RenderReadPixels(context->renderer, &info->clip_rect, 
            info->format->format, pixbuf, info->w * info->format->BytesPerPixel);
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
