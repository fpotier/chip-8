#include <vector>
#include <cstdint>

#include "chip8.h"
#include "config.h"
#include "sdl_helper.h"
#include "widget/widget.h"

class App
{
public:
    static constexpr uint32_t init_flags = SDL_INIT_AUDIO | SDL_INIT_EVENTS| SDL_INIT_TIMER | SDL_INIT_VIDEO;

    App(Config& conf, std::string const& rom_path, std::vector<uint8_t> const& program);
    ~App();
    int exec();

    static std::vector<uint8_t> load_rom(std::filesystem::path rom_path);

private:
    void handle_event();
    void render();
    void play_sound();

    Chip8 m_emulator;
    std::string const& m_rom_path;
    Config m_conf;
    SDLUniqueWindow m_window;
    SDLSharedRenderer m_renderer;
    SDL_Event m_event;
    bool m_quit;
    bool m_audio_enabled;
    uint32_t m_wavfile_length;
    uint8_t* m_wav_buffer;
    SDL_AudioSpec m_wav_spec;
    SDL_AudioDeviceID m_audio_device;
    TTFSharedFont m_font;

    std::vector<widget_ptr> m_widgets;
};
