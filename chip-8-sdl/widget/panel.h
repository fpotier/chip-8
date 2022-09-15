#pragma once

#include <vector>

#include "widget.h"

class panel : public widget
{
public:
    panel(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color)
        : widget(renderer, x, y, w, h, fg_color, bg_color),
        m_children()
    {}
    ~panel() override;
    void draw() override;
    void add_child(widget_ptr child);
private:
    std::vector<widget_ptr> m_children;
};

using panel_ptr = std::shared_ptr<panel>;
