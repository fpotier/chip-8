#include <fmt/core.h>
#include <fstream>
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

void app::handle_event()
{
    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_QUIT)
                m_quit = true;
        else if (m_event.type == SDL_KEYDOWN)
        {
            switch (m_event.key.keysym.sym)
            {
                case SDLK_1: m_emulator.key_pressed(1); break;
                case SDLK_2: m_emulator.key_pressed(2); break;
                case SDLK_3: m_emulator.key_pressed(3); break;
                case SDLK_4: m_emulator.key_pressed(12); break;
                case SDLK_a: m_emulator.key_pressed(4); break;
                case SDLK_z: m_emulator.key_pressed(5); break;
                case SDLK_e: m_emulator.key_pressed(6); break;
                case SDLK_r: m_emulator.key_pressed(13); break;
                case SDLK_q: m_emulator.key_pressed(7); break;
                case SDLK_s: m_emulator.key_pressed(8); break;
                case SDLK_d: m_emulator.key_pressed(9); break;
                case SDLK_f: m_emulator.key_pressed(14); break;
                case SDLK_w: m_emulator.key_pressed(10); break;
                case SDLK_x: m_emulator.key_pressed(0); break;
                case SDLK_c: m_emulator.key_pressed(11); break;
                case SDLK_v: m_emulator.key_pressed(15); break;
                case SDLK_ESCAPE: m_quit = true; break;
            }
        }
        else if (m_event.type == SDL_KEYUP)
        {
            switch (m_event.key.keysym.sym)
            {
                case SDLK_1: m_emulator.key_released(1); break;
                case SDLK_2: m_emulator.key_released(2); break;
                case SDLK_3: m_emulator.key_released(3); break;
                case SDLK_4: m_emulator.key_released(12); break;
                case SDLK_a: m_emulator.key_released(4); break;
                case SDLK_z: m_emulator.key_released(5); break;
                case SDLK_e: m_emulator.key_released(6); break;
                case SDLK_r: m_emulator.key_released(13); break;
                case SDLK_q: m_emulator.key_released(7); break;
                case SDLK_s: m_emulator.key_released(8); break;
                case SDLK_d: m_emulator.key_released(9); break;
                case SDLK_f: m_emulator.key_released(14); break;
                case SDLK_w: m_emulator.key_released(10); break;
                case SDLK_x: m_emulator.key_released(0); break;
                case SDLK_c: m_emulator.key_released(11); break;
                case SDLK_v: m_emulator.key_released(15); break;
            }
        }
    }
}

void app::render()
{
    std::array<uint8_t, chip8::vram_size> const& emulator_vram = m_emulator.get_vram();
    if (m_emulator.vram_dirty)
    {
        m_pixel.x = 0; m_pixel.y = 0;
        SDL_RenderClear(m_renderer);
        for (std::size_t y = 0; y < chip8::screen_height; y++)
        {
            m_pixel.y = y * m_pixel.h;
            for (std::size_t x = 0; x < chip8::screen_width; x++)
            {
                m_pixel.x = x * m_pixel.w;
                uint8_t pixel_val = emulator_vram[x + y * chip8::screen_width];
                if (pixel_val)
                    set_renderer_color(m_conf.fg_color);
                else
                    set_renderer_color(m_conf.bg_color);

                SDL_RenderFillRect(m_renderer, &m_pixel);
            }
        }
        SDL_RenderPresent(m_renderer);
    }
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

std::vector<uint8_t> app::load_rom(std::filesystem::path rom_path)
{
    std::ifstream rom_file(rom_path.string(), std::ios::binary);
    if (!rom_file)
    {
        std::cerr << fmt::format("Unable to read ROM: {}\n", rom_path.string());
        exit(EXIT_FAILURE);
    }
    std::streampos rom_size = 0;

    rom_file.seekg(0, std::ios::end);
    rom_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom_content(rom_size);
    rom_file.read(reinterpret_cast<char*>(rom_content.data()), rom_size);

    return rom_content;
}
