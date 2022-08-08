#include <iostream>

#include "app.h"
#include "chip8.h"

app::app(config& conf)
{
    m_conf = conf;
    m_window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        conf.window_width, conf.window_height, 0);
    sdl_nullcheck(m_window);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    sdl_nullcheck(m_renderer);
}

app::~app()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

template <typename T>
void app::sdl_nullcheck(T ptr)
{
    if (!ptr)
    {
        std::cerr << SDL_GetError() << '\n';
        abort();
    }
}
