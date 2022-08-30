#include "sdl_helper.h"
#include "widget.h"

widget::widget(SDL_Renderer* renderer, int x, int y, int width, int height)
    : m_rect { x, y, width, height }
{
    m_renderer = renderer;
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h);
    sdl_nullcheck(m_texture, "Failed to create the texture: %s\n");
}

widget::~widget()
{
    SDL_DestroyTexture(m_texture);
}

void widget::as_rendering_target() const
{
   sdl_checksuccess(SDL_SetRenderTarget(m_renderer, m_texture), "Failed to set the widget as rendering target: %s\n");
}
