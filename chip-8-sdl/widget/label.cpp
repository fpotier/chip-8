#include "label.h"
#include "sdl_helper.h"

label::~label()
{}

void label::draw()
{
    if (m_font == nullptr)
        return;

    as_rendering_target();

    SDL_FreeSurface(m_surface);
    m_surface = TTF_RenderText_Solid(m_font, m_content.c_str(), m_fg_color);
    sdl_nullcheck(m_surface, "Failed to render label: %s\n");
    SDL_Texture* tmp_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    SDL_RenderCopy(m_renderer, tmp_texture, NULL, NULL);
    SDL_DestroyTexture(tmp_texture);
}
