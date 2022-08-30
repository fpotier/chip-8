#pragma once

#include <string>
#include <SDL_ttf.h>

#include "widget.h"

class label : public widget
{
public:
    label(SDL_Renderer* renderer, int x, int y, int width, int height, std::string content, TTF_Font* font, SDL_Color fg_color, SDL_Color bg_color)
        : widget(renderer, x, y, width, height), m_content(content), m_font(font)
        , m_fg_color(fg_color), m_bg_color(bg_color)
    {}
    ~label() override;
    void draw() override;
private:
    std::string m_content;
    TTF_Font* m_font;
    SDL_Surface* m_surface = nullptr;
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
};
