#pragma once

#include <array>

#include "widget.h"

using icon = std::array<uint8_t, 1024>;

class Button : public Widget
{
public:
    Button(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color, icon const& icon_data, int icon_w, int icon_h)
        : Widget(renderer, x, y, w, h, fg_color, bg_color),
        m_icon(icon_data), m_icon_w(icon_w), m_icon_h(icon_h)
    {}
    ~Button() override;
    void draw() override;
private:
    icon const& m_icon;
    int m_icon_w;
    int m_icon_h;
};

using button_ptr = std::unique_ptr<Button>;
