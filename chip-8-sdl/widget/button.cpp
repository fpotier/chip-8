#include "button.h"

button::~button()
{}

void button::draw()
{
    as_rendering_target();

    for (int j = 0; j < m_icon_h; j++)
    {
        for (int i = 0; i < m_icon_w; i++)
        {
            set_renderer_color(m_renderer, m_icon[i + m_icon_w * j]
                ? m_fg_color : m_bg_color);
            SDL_RenderDrawPoint(m_renderer.get(), i, j);
        }
    }
}
