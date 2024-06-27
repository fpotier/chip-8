#pragma once

#include "chip8.h"
#include "widget.h"

class Chip8Screen : public Widget
{
public:
    Chip8Screen(SDLSharedRenderer renderer, int x, int y, SDL_Color fg_color, SDL_Color bg_color, Chip8 const& emulator, int SCALE_FACTOR)
        : Widget(renderer, x, y, Chip8::SCREEN_WIDTH * SCALE_FACTOR, Chip8::SCREEN_HEIGHT * SCALE_FACTOR, fg_color, bg_color),
        m_emulator(emulator), m_scale_factor(SCALE_FACTOR)
    {}
    ~Chip8Screen() override;
    void draw() override;
private:
    Chip8 const& m_emulator;
    int m_scale_factor;
};
