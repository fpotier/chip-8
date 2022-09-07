#pragma once

#include <vector>

#include "widget.h"

class panel : public widget
{
public:
    panel(SDLSharedRenderer renderer, int x, int y, int width, int height, SDL_Color fg_color, SDL_Color bg_color)
        : widget(renderer, x, y, width, height),
        m_fg_color(fg_color), m_bg_color(bg_color), m_children()
    {}
    ~panel() override;
    void draw() override;
    void add_child(widget_ptr child);
private:
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
    std::vector<widget_ptr> m_children;
};

using panel_ptr = std::shared_ptr<panel>;
