#pragma once


#include "sdl_helper.h"

class widget
{
public:
    widget(SDLSharedRenderer renderer, int x, int y, int width, int height);
    virtual ~widget();
    void as_rendering_target() const;
    virtual void draw() = 0;

    SDL_Rect* rect() { return &m_rect; }
    SDL_Texture* texture() const { return m_texture.get(); }
protected:
    SDL_Rect m_rect;
    SDLSharedRenderer m_renderer;
    SDLUniqueTexture m_texture;
};

using widget_ptr = std::shared_ptr<widget>;
