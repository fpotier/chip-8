#include "chip8_screen.h"
#include "sdl_helper.h"

chip8_screen::~chip8_screen()
{}

void chip8_screen::draw()
{
    as_rendering_target();

    set_renderer_color(m_renderer, m_bg_color);
    SDL_RenderClear(m_renderer);
    std::array<uint8_t, chip8::vram_size> const& emulator_vram = m_emulator.get_vram();
    if (m_emulator.vram_dirty)
    {
        for (std::size_t y = 0; y < chip8::screen_height; y++)
        {
            for (std::size_t x = 0; x < chip8::screen_width; x++)
            {
                uint8_t pixel_val = emulator_vram[x + y * chip8::screen_width];
                if (pixel_val)
                    set_renderer_color(m_renderer, m_fg_color);
                else
                    set_renderer_color(m_renderer, m_bg_color);

                SDL_RenderDrawPoint(m_renderer, x, y);
            }
        }
    }
}