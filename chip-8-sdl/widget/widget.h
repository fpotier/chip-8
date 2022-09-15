#pragma once


#include "sdl_helper.h"

class widget
{
public:
    widget(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color);
    virtual ~widget();
    void as_rendering_target() const;
    virtual void draw() = 0;

    SDL_Rect* rect() { return &m_rect; }
    SDL_Texture* texture() const { return m_texture.get(); }
    int x() { return m_rect.x; }
    int y() { return m_rect.y; }
    int w() { return m_rect.w; }
    int h() { return m_rect.h; }
protected:
    SDL_Rect m_rect;
    SDLSharedRenderer m_renderer;
    SDLUniqueTexture m_texture;
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
};

using widget_ptr = std::shared_ptr<widget>;
