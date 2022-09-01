#pragma once

#include <cstdlib>
#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <type_traits>

struct SDLCleaner
{
    void operator()(SDL_Window* window);
    void operator()(SDL_Renderer* renderer);
    void operator()(SDL_Surface* surface);
    void operator()(SDL_Texture* texture);
    void operator()(TTF_Font* font);
};

using SDLUniqueWindow = std::unique_ptr<SDL_Window, SDLCleaner>;
using SDLSharedRenderer = std::shared_ptr<SDL_Renderer>;
using SDLUniqueTexture = std::unique_ptr<SDL_Texture, SDLCleaner>;
using SDLUniqueSurface = std::unique_ptr<SDL_Surface, SDLCleaner>;
using TTFSharedFont = std::shared_ptr<TTF_Font>;

template <typename T>
void sdl_nullcheck(T ptr, const char* fmt)
{
    static_assert(std::is_pointer<T>(), "T should be a pointer");
    if (!ptr)
    {
        SDL_Log(fmt, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void sdl_checksuccess(int ret_val, const char* fmt);
void set_renderer_color(SDLSharedRenderer renderer, SDL_Color col);
