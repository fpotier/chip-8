#include "sdl_helper.h"
#include "widget.h"

Widget::Widget(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color)
    : m_rect { x, y, w, h }, m_fg_color(fg_color), m_bg_color(bg_color)
{
    m_renderer = renderer;
    m_texture = SDLUniqueTexture(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h), SDLCleaner());
    sdl_nullcheck(m_texture.get(), "Failed to create the texture: %s\n");
}

Widget::~Widget()
{}

void Widget::as_rendering_target() const
{
   sdl_checksuccess(SDL_SetRenderTarget(m_renderer.get(), m_texture.get()), "Failed to set the widget as rendering target: %s\n");
}
