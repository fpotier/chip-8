#include "chip8_screen.h"
#include "sdl_helper.h"

Chip8Screen::~Chip8Screen()
{}

void Chip8Screen::draw()
{
    as_rendering_target();

    set_renderer_color(m_renderer, m_bg_color);
    SDL_RenderClear(m_renderer.get());
    std::array<uint8_t, Chip8::VRAM_SIZE> const& emulator_vram = m_emulator.get_vram();
    if (m_emulator.vram_dirty)
    {
        SDL_Rect pixel = SDL_Rect { 0, 0, m_scale_factor, m_scale_factor };
        for (std::size_t y = 0; y < Chip8::SCREEN_HEIGHT; y++)
        {
            pixel.y = y * pixel.h;
            for (std::size_t x = 0; x < Chip8::SCREEN_WIDTH; x++)
            {
                pixel.x = x * pixel.w;
                uint8_t pixel_val = emulator_vram[x + y * Chip8::SCREEN_WIDTH];
                if (pixel_val)
                    set_renderer_color(m_renderer, m_fg_color);
                else
                    set_renderer_color(m_renderer, m_bg_color);

                SDL_RenderFillRect(m_renderer.get(), &pixel);
            }
        }
    }
}