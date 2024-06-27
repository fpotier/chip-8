#pragma once


#include "sdl_helper.h"

class Widget
{
public:
    Widget(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color);
    virtual ~Widget();
    void as_rendering_target() const;
    virtual void draw() = 0;

    SDL_Rect* rect() { return &m_rect; }
    SDL_Texture* texture() const { return m_texture.get(); }
    int x() const { return m_rect.x; }
    int& x() { return m_rect.x; }
    int y() const { return m_rect.y; }
    int& y() { return m_rect.y; }
    int w() const { return m_rect.w; }
    int& w() { return m_rect.w; }
    int h() const { return m_rect.h; }
    int& h() { return m_rect.h; }
protected:
    SDL_Rect m_rect;
    SDLSharedRenderer m_renderer;
    SDLUniqueTexture m_texture;
    SDL_Color m_fg_color;
    SDL_Color m_bg_color;
};

using widget_ptr = std::unique_ptr<Widget>;
