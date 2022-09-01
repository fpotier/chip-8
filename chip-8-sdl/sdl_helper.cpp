#include <iostream>

#include "sdl_helper.h"

void sdl_checksuccess(int ret_val, const char* fmt)
{
    if (ret_val)
    {
        SDL_Log(fmt, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void set_renderer_color(SDLSharedRenderer renderer, SDL_Color col)
{
    sdl_checksuccess(SDL_SetRenderDrawColor(renderer.get(), col.r, col.g, col.b, col.a), "Failed to set renderer color: %s\n");
}

void SDLCleaner::operator()(SDL_Window* window)
{
    std::cout << "Destroy window\n";
    SDL_DestroyWindow(window);
}

void SDLCleaner::operator()(SDL_Renderer* renderer)
{
    std::cout << "Destroy renderer\n";
    SDL_DestroyRenderer(renderer);
}

void SDLCleaner::operator()(SDL_Surface* surface)
{
    std::cout << "Destroy surface\n";
    SDL_FreeSurface(surface);
}

void SDLCleaner::operator()(SDL_Texture* texture)
{
    std::cout << "Destroy texture\n";
    SDL_DestroyTexture(texture);
}

void SDLCleaner::operator()(TTF_Font* font)
{
    std::cout << "Destroy font\n";
    TTF_CloseFont(font);
    // FIXME: This is safe to do for now since we only have one font
    // But SDL_Init/Close() and TTF_Init/Close() should probably be called from main()
    TTF_Quit();
}
