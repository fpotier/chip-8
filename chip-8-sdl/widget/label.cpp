#include "label.h"
#include "sdl_helper.h"

Label::~Label()
{}

void Label::draw()
{
    if (!m_changed || !m_font.get())
        return;

    as_rendering_target();

    m_surface = SDLUniqueSurface(TTF_RenderText_Solid(m_font.get(), m_content.c_str(), m_fg_color), SDLCleaner());
    sdl_nullcheck(m_surface.get(), "Failed to render label: %s\n");
    SDLUniqueTexture tmp_texture = SDLUniqueTexture(SDL_CreateTextureFromSurface(m_renderer.get(), m_surface.get()), SDLCleaner());
    SDL_RenderCopy(m_renderer.get(), tmp_texture.get(), NULL, NULL);
    m_changed = false;
}
