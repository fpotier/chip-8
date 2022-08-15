#include <SDL.h>
#include <vector>

#include "chip8.h"
#include "config.h"

class app
{
public:
    static constexpr uint32_t init_flags = SDL_INIT_AUDIO | SDL_INIT_EVENTS| SDL_INIT_TIMER | SDL_INIT_VIDEO;

    app(config& conf, const uint8_t* program, size_t program_size);
    ~app();
    int exec();

    static std::vector<uint8_t> load_rom(std::filesystem::path rom_path);

private:
    void handle_event();
    void render();
    void set_renderer_color(SDL_Color col);

    chip8 m_emulator;
    config m_conf;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Rect m_pixel;
    SDL_Event m_event;
    bool m_quit;

    template <typename T>
    static void sdl_nullcheck(T ptr, const char* fmt);
    static void sdl_checksuccess(int ret_val, const char* fmt);
};