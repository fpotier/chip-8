#include <SDL.h>

#include "config.h"

class app
{
public:
    app(config& conf);
    ~app();
    void loop();
    bool handle_event();
    void render();

private:
    config m_conf;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Rect m_pixel;

    template <typename T>
    static void sdl_nullcheck(T ptr);
};
