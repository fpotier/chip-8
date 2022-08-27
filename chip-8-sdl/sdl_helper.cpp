#include "sdl_helper.h"

void sdl_checksuccess(int ret_val, const char* fmt)
{
    if (ret_val)
    {
        SDL_Log(fmt, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void set_renderer_color(SDL_Renderer* renderer, SDL_Color col)
{
    sdl_checksuccess(SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a), "Failed to set renderer color: %s\n");
}