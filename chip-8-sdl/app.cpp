#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <SDL_ttf.h>

#include "app.h"
#include "sdl_helper.h"
#include "widget/chip8_screen.h"
#include "widget/label.h"
#include "widget/panel.h"

static constexpr int font_point_size = 12;

static constexpr int scale_factor = 10;
static constexpr int panel_width = chip8::screen_width * scale_factor;
static constexpr int panel_height = (chip8::screen_height / 8) * scale_factor;
static constexpr int renderer_width = chip8::screen_width * scale_factor;
static constexpr int renderer_height = chip8::screen_height * scale_factor + panel_height;
static constexpr int chip8screen_y = panel_height;

app::app(config& conf, std::string const& rom_path, const uint8_t* program, size_t program_size)
    : m_emulator(program, program_size), m_rom_path(rom_path)
{
    sdl_checksuccess(SDL_Init(init_flags), "Failed to initialize the SDL: %s\n");
    sdl_checksuccess(TTF_Init(), "Failed to initialize SDL_ttf: %s\n");
    m_conf = conf;
    m_window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        conf.window_width, conf.window_height, 0);
    sdl_nullcheck(m_window, "Failed to create the window: %s\n");
    SDL_SetWindowResizable(m_window, SDL_TRUE);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    sdl_nullcheck(m_renderer, "Failed to create the renderer: %s\n");
    set_renderer_color(m_renderer, m_conf.bg_color);
    sdl_checksuccess(SDL_RenderClear(m_renderer), "Failed to clear the renderer: %s\n");
    sdl_checksuccess(SDL_RenderSetLogicalSize(m_renderer, renderer_width, renderer_height),
        "Failed to set the renderer's logical scale: %s");
    sdl_checksuccess(SDL_RenderSetIntegerScale(m_renderer, SDL_TRUE),
        "Failed to set integer scaling on the renderer: %s");
    m_quit = false;
    m_audio_enabled = false;
    m_audio_device = 0;
    m_wav_buffer = nullptr;
    if (conf.sound_file)
    {
        std::string sound_strpath = conf.sound_file.value().string();
        std::cout << sound_strpath << '\n';
        SDL_LoadWAV(sound_strpath.c_str(),
            &m_wav_spec, &m_wav_buffer, &m_wavfile_length);
        // FIXME: fmt string
        sdl_nullcheck(&m_wav_spec, fmt::format("Failed to open the sound file: {}\n", sound_strpath).c_str());
        m_audio_device = SDL_OpenAudioDevice(nullptr, 0, &m_wav_spec, nullptr, 0);
        // NOTE: Maybe create a SDL wrapper if more function return 0 on error
        if (!m_audio_device)
        {
            SDL_FreeWAV(m_wav_buffer);
            SDL_Log("Failed to open audio device: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        m_audio_enabled = true;
    }
    m_font = nullptr;
    if (m_conf.font_file)
    {
        std::string font_strpath = m_conf.font_file.value().string();
        m_font = TTF_OpenFont(font_strpath.c_str(), font_point_size);
        // FIXME: fmt string
        sdl_nullcheck(m_font, fmt::format("Failed to open font file: {}", font_strpath).c_str());
    }

    panel* p1 = new panel(m_renderer, 0, 0, panel_width, panel_height, m_conf.fg_color, m_conf.bg_color);
    panel* p2 = new panel(m_renderer, 1, 1, panel_width / 2, panel_height / 2, m_conf.fg_color, m_conf.bg_color);
    label* l1 = new label(m_renderer, 1, 2, panel_width - 2, panel_height - 3, m_rom_path, m_font, m_conf.fg_color, m_conf.bg_color);
    p1->add_child(l1);
    //p1->add_child(p2);

    m_widgets.push_back(p1);
    m_widgets.push_back(new chip8_screen(m_renderer, 0, panel_height, m_emulator, m_conf.fg_color, m_conf.bg_color, scale_factor));
}

app::~app()
{

    if (m_audio_device > 0)
        SDL_CloseAudioDevice(m_audio_device);
    SDL_FreeWAV(m_wav_buffer); // Safe to call on nullptr
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_CloseFont(m_font);
    TTF_Quit();
    SDL_Quit();
}

// FIXME: this should return EXIT_FAILURE when something goes wrong
int app::exec()
{
    while (!m_quit)
    {
        uint64_t frame_start = SDL_GetPerformanceCounter();

        handle_event();
        m_emulator.tick(m_conf.instructions_per_frame);
        render();
        if (m_emulator.get_sound_timer() == 1)
            play_sound();

        uint64_t frame_end = SDL_GetPerformanceCounter();
        float elapsed_ms = (frame_end - frame_start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;
        float to_wait = std::floor(16.6f - elapsed_ms);
        if (to_wait > 0)
            SDL_Delay(to_wait);
    }
    return EXIT_SUCCESS;
}

void app::handle_event()
{
    while (SDL_PollEvent(&m_event))
    {
        if (m_event.type == SDL_QUIT)
                m_quit = true;
        else if (m_event.type == SDL_KEYDOWN)
        {
            switch (m_event.key.keysym.sym)
            {
                case SDLK_1: m_emulator.key_pressed(1); break;
                case SDLK_2: m_emulator.key_pressed(2); break;
                case SDLK_3: m_emulator.key_pressed(3); break;
                case SDLK_4: m_emulator.key_pressed(12); break;
                case SDLK_a: m_emulator.key_pressed(4); break;
                case SDLK_z: m_emulator.key_pressed(5); break;
                case SDLK_e: m_emulator.key_pressed(6); break;
                case SDLK_r: m_emulator.key_pressed(13); break;
                case SDLK_q: m_emulator.key_pressed(7); break;
                case SDLK_s: m_emulator.key_pressed(8); break;
                case SDLK_d: m_emulator.key_pressed(9); break;
                case SDLK_f: m_emulator.key_pressed(14); break;
                case SDLK_w: m_emulator.key_pressed(10); break;
                case SDLK_x: m_emulator.key_pressed(0); break;
                case SDLK_c: m_emulator.key_pressed(11); break;
                case SDLK_v: m_emulator.key_pressed(15); break;
                case SDLK_ESCAPE: m_quit = true; break;
            }
        }
        else if (m_event.type == SDL_KEYUP)
        {
            switch (m_event.key.keysym.sym)
            {
                case SDLK_1: m_emulator.key_released(1); break;
                case SDLK_2: m_emulator.key_released(2); break;
                case SDLK_3: m_emulator.key_released(3); break;
                case SDLK_4: m_emulator.key_released(12); break;
                case SDLK_a: m_emulator.key_released(4); break;
                case SDLK_z: m_emulator.key_released(5); break;
                case SDLK_e: m_emulator.key_released(6); break;
                case SDLK_r: m_emulator.key_released(13); break;
                case SDLK_q: m_emulator.key_released(7); break;
                case SDLK_s: m_emulator.key_released(8); break;
                case SDLK_d: m_emulator.key_released(9); break;
                case SDLK_f: m_emulator.key_released(14); break;
                case SDLK_w: m_emulator.key_released(10); break;
                case SDLK_x: m_emulator.key_released(0); break;
                case SDLK_c: m_emulator.key_released(11); break;
                case SDLK_v: m_emulator.key_released(15); break;
            }
        }
    }
}

void app::render()
{
    set_renderer_color(m_renderer, m_conf.bg_color);
    SDL_RenderClear(m_renderer);
    for (widget* widget : m_widgets)
    {
        widget->draw();
        SDL_SetRenderTarget(m_renderer, NULL);
        SDL_RenderCopy(m_renderer, widget->texture(), NULL, widget->rect());
    }
    SDL_RenderPresent(m_renderer);
}

void app::play_sound()
{
    if (m_audio_enabled)
    {
        sdl_checksuccess(SDL_QueueAudio(m_audio_device, m_wav_buffer, m_wavfile_length),
            "Failed to play sound");
        SDL_PauseAudioDevice(m_audio_device, false);
    }
}

std::vector<uint8_t> app::load_rom(std::filesystem::path rom_path)
{
    std::ifstream rom_file(rom_path.string(), std::ios::binary);
    if (!rom_file)
    {
        std::cerr << fmt::format("Unable to read ROM: {}\n", rom_path.string());
        exit(EXIT_FAILURE);
    }
    std::streampos rom_size = 0;

    rom_file.seekg(0, std::ios::end);
    rom_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom_content(rom_size);
    rom_file.read(reinterpret_cast<char*>(rom_content.data()), rom_size);

    return rom_content;
}
