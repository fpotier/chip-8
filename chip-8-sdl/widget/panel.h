#pragma once

#include <vector>

#include "widget.h"
#include "layout.h"

class Panel : public Widget
{
public:
    Panel(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color, Layout layout = Layout::NoLayout)
        : Widget(renderer, x, y, w, h, fg_color, bg_color),
        m_children(),
        m_layout(layout)
    {}
    ~Panel() override;
    void draw() override;
    void add_child(widget_ptr&& child);
    void set_layout(Layout layout);
private:
    void apply_layout();

    std::vector<widget_ptr> m_children;
    Layout m_layout = Layout::NoLayout;
};

using panel_ptr = std::unique_ptr<Panel>;
