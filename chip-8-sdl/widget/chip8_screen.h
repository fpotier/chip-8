#pragma once

#include "chip8.h"
#include "widget.h"

class chip8_screen : public widget
{
public:
    chip8_screen(SDLSharedRenderer renderer, int x, int y, SDL_Color fg_color, SDL_Color bg_color, chip8 const& emulator, int scale_factor)
        : widget(renderer, x, y, chip8::screen_width * scale_factor, chip8::screen_height * scale_factor, fg_color, bg_color),
        m_emulator(emulator), m_scale_factor(scale_factor)
    {}
    ~chip8_screen() override;
    void draw() override;
private:
    chip8 const& m_emulator;
    int m_scale_factor;
};
