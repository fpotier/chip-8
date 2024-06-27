#include <array>

#include "panel.h"
#include "sdl_helper.h"

Panel::~Panel()
{}

void Panel::draw()
{
    as_rendering_target();

    std::array<SDL_Point, 5> edges {
        SDL_Point { 0, 0 },
        SDL_Point { m_rect.w - 1, 0 },
        SDL_Point { m_rect.w - 1, m_rect.h - 1},
        SDL_Point { 0, m_rect.h - 1},
        SDL_Point { 0, 0 },
    };
    set_renderer_color(m_renderer, m_bg_color);
    SDL_RenderClear(m_renderer.get());
    set_renderer_color(m_renderer, m_fg_color);
    SDL_RenderDrawLines(m_renderer.get(), edges.data(), edges.size());

    for (widget_ptr const& widget : m_children)
    {
        widget->draw();
        as_rendering_target();
        SDL_RenderCopy(m_renderer.get(), widget->texture(), NULL, widget->rect());
    }
}

void Panel::add_child(widget_ptr&& child)
{
    m_children.push_back(std::move(child));
    apply_layout();
}

void Panel::set_layout(Layout l)
{
    m_layout = l;
    apply_layout();
}

void Panel::apply_layout()
{
    switch (m_layout) {
        case Layout::Horizontal:
        {
            int next_x = 2;
            for (widget_ptr& widget : m_children)
            {
                widget->x() = next_x;
                next_x += widget->w();
            }
            break;
        }
        case Layout::Vertical:
        {
            int next_y = 0;
            for (widget_ptr& widget : m_children)
            {
                widget->y() = next_y;
                next_y += widget->h();
            }
            break;
        }
        case Layout::NoLayout:
            break;
    }
}
