#include "sdl_helper.h"
#include "widget.h"

widget::widget(SDLSharedRenderer renderer, int x, int y, int width, int height)
    : m_rect { x, y, width, height }
{
    m_renderer = renderer;
    m_texture = SDLUniqueTexture(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h), SDLCleaner());
    sdl_nullcheck(m_texture.get(), "Failed to create the texture: %s\n");
}

widget::~widget()
{}

void widget::as_rendering_target() const
{
   sdl_checksuccess(SDL_SetRenderTarget(m_renderer.get(), m_texture.get()), "Failed to set the widget as rendering target: %s\n");
}
