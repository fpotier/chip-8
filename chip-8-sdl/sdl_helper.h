#pragma once

#include <cstdlib>
#include <SDL.h>
#include <type_traits>

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
void set_renderer_color(SDL_Renderer* renderer, SDL_Color col);