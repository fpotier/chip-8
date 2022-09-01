#pragma once

#include <string>

#include "widget.h"

class label : public widget
{
public:
    label(SDLSharedRenderer renderer, int x, int y, int width, int height, std::string content, TTFSharedFont font, SDL_Color fg_color, SDL_Color bg_color)
        : widget(renderer, x, y, width, height), m_content(content), m_font(font)
        , m_fg_color(fg_color), m_bg_color(bg_color), m_changed(true)
    {}
    ~label() override;
    void draw() override;
private:
    std::string m_content;
    TTFSharedFont m_font;
    SDLUniqueSurface m_surface;
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
    bool m_changed;
};
