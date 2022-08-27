#pragma once

#include "chip8.h"
#include "widget.h"

class chip8_screen : public widget
{
public:
    chip8_screen(SDL_Renderer* renderer, int x, int y, chip8 const& emulator, SDL_Color fg_color, SDL_Color bg_color)
        : widget(renderer, x, y, chip8::screen_width, chip8::screen_height),
        m_emulator(emulator), m_fg_color(fg_color), m_bg_color(bg_color)
    {}
    ~chip8_screen();
    void draw() override;
private:
    chip8 const& m_emulator;
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
};
