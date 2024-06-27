#pragma once

#include <string>

#include "widget.h"

class Label : public Widget
{
public:
    Label(SDLSharedRenderer renderer, int x, int y, int w, int h, SDL_Color fg_color, SDL_Color bg_color, const std::string content, TTFSharedFont font)
        : Widget(renderer, x, y, w, h, fg_color, bg_color),
        m_content(content), m_font(font), m_changed(true)
    {}
    ~Label() override;
    void draw() override;
private:
    std::string m_content;
    TTFSharedFont m_font;
    SDLUniqueSurface m_surface;
    bool m_changed;
};

using label_ptr = std::unique_ptr<Label>;
