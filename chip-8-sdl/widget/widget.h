#pragma once

#include <cstdint>
#include <SDL.h>

class widget
{
public:
    widget(SDL_Renderer* renderer, int x, int y, int width, int height);
    virtual ~widget();
    void as_rendering_target();
    SDL_Texture* texture() { return m_texture; }
    SDL_Rect* rect() { return &m_rect; }
    virtual void draw() = 0;
protected:
    SDL_Rect m_rect;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
};
