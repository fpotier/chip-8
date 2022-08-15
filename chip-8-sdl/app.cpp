#include <iostream>

#include "app.h"

app::app(config& conf, const uint8_t* program, size_t program_size)
    : m_emulator(program, program_size)
{
    sdl_checksuccess(SDL_Init(init_flags), "Failed to initialize the SDL: %s");
    m_conf = conf;
    m_window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        conf.window_width, conf.window_height, 0);
    sdl_nullcheck(m_window, "Failed to create the window: %s\n");
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    sdl_nullcheck(m_renderer, "Failed to create the renderer: %s\n");
    set_renderer_color(m_conf.bg_color);
    sdl_checksuccess(SDL_RenderClear(m_renderer), "Failed to clear the renderer: %s\n");
    m_pixel.w = conf.window_width / chip8::screen_width;
    m_pixel.h = conf.window_height / chip8::screen_height;
    m_quit = false;
}

app::~app()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

int app::exec()
{
    while (!m_quit)
    {
        handle_event();
        // FIXME: tick should be configurable
        m_emulator.tick(9);
        render();
    }
    return EXIT_SUCCESS;
}

void app::set_renderer_color(SDL_Color col)
{
    sdl_checksuccess(SDL_SetRenderDrawColor(m_renderer, col.r, col.g, col.b, col.a), "Failed to set renderer color: %s\n");
}

template <typename T>
void app::sdl_nullcheck(T ptr, const char* fmt)
{
    static_assert(std::is_pointer<T>(), "T should be a pointer");
    if (!ptr)
    {
        SDL_Log(fmt, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void app::sdl_checksuccess(int ret_val, const char* fmt)
{
    if (ret_val)
    {
        SDL_Log(fmt, SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
