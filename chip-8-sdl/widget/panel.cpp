#include <array>

#include "panel.h"
#include "sdl_helper.h"

panel::~panel()
{}

void panel::draw()
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

    for (widget_ptr widget : m_children)
    {
        widget->draw();
        as_rendering_target();
        SDL_RenderCopy(m_renderer.get(), widget->texture(), NULL, widget->rect());
    }
}

void panel::add_child(widget_ptr child)
{
    m_children.push_back(child);
}
