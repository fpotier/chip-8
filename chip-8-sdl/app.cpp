#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <istream>

#include "app.h"
#include "icons/folder.h"
#include "icons/play.h"
#include "icons/warning.h"
#include "widget/button.h"
#include "widget/chip8_screen.h"
#include "widget/label.h"
#include "widget/panel.h"

static constexpr int font_point_size = 12;

static constexpr int SCALE_FACTOR = 10;
static constexpr int PANEL_WIDTH = Chip8::SCREEN_WIDTH * SCALE_FACTOR;
static constexpr int PANEL_HEIGHT = (Chip8::SCREEN_HEIGHT / 8) * SCALE_FACTOR;
static constexpr int RENDERER_WIDTH = Chip8::SCREEN_WIDTH * SCALE_FACTOR;
static constexpr int RENDERER_HEIGHT = Chip8::SCREEN_HEIGHT * SCALE_FACTOR + PANEL_HEIGHT;
static constexpr int CHIP8SCREEN_Y = PANEL_HEIGHT;

App::App(Config& conf, std::string const& rom_path, std::vector<uint8_t> const& program)
    : m_emulator(program),
      m_rom_path(rom_path),
      m_conf(conf),
      m_window(SDLUniqueWindow(SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        conf.window_width, conf.window_height, 0) , SDLCleaner())),
      m_quit(false),
      m_audio_enabled(false),
      m_wav_buffer(nullptr),
      m_audio_device(0)
{
    sdl_checksuccess(SDL_Init(init_flags), "Failed to initialize the SDL: %s\n");
    sdl_checksuccess(TTF_Init(), "Failed to initialize SDL_ttf: %s\n");
    sdl_nullcheck(m_window.get(), "Failed to create the window: %s\n");
    SDL_SetWindowResizable(m_window.get(), SDL_TRUE);
    m_renderer = SDLSharedRenderer(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED), SDLCleaner());
    sdl_nullcheck(m_renderer.get(), "Failed to create the renderer: %s\n");
    set_renderer_color(m_renderer, m_conf.bg_color);
    sdl_checksuccess(SDL_RenderClear(m_renderer.get()), "Failed to clear the renderer: %s\n");
    // sdl_checksuccess(SDL_RenderSetLogicalSize(m_renderer.get(), RENDERER_WIDTH, RENDERER_HEIGHT),
        // "Failed to set the renderer's logical scale: %s");
    sdl_checksuccess(SDL_RenderSetIntegerScale(m_renderer.get(), SDL_TRUE),
        "Failed to set integer scaling on the renderer: %s");
    if (conf.sound_file)
    {
        std::string sound_strpath = conf.sound_file.value().string();
        fmt::print("Sound path: {}\n", sound_strpath);
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
    if (m_conf.font_file)
    {
        std::string font_strpath = m_conf.font_file.value().string();
        m_font = TTFSharedFont(TTF_OpenFont(font_strpath.c_str(), font_point_size), SDLCleaner());
        // FIXME: fmt string
        sdl_nullcheck(m_font.get(), fmt::format("Failed to open font file: {}", font_strpath).c_str());
    }

    panel_ptr p1 = std::make_unique<Panel>(m_renderer, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, m_conf.fg_color, m_conf.bg_color, Layout::Horizontal);
    p1->add_child(std::make_unique<Button>(m_renderer, 1, 2, 32, 32, m_conf.fg_color, m_conf.bg_color, folder_icon, 32, 32));
    p1->add_child(std::make_unique<Button>(m_renderer, 1, 2, 32, 32, m_conf.fg_color, m_conf.bg_color, play_icon, 32, 32));
    p1->add_child(std::make_unique<Button>(m_renderer, 1, 2, 32, 32, m_conf.fg_color, m_conf.bg_color, warning_icon, 32, 32));
    // label_ptr l1 = std::make_shared<Label>(m_renderer, 1, 2, PANEL_WIDTH - 2, PANEL_HEIGHT - 3,  m_conf.fg_color, m_conf.bg_color, m_rom_path, m_font);
    // p1->add_child(l1);
    m_widgets.push_back(std::make_unique<Chip8Screen>(m_renderer, 0, PANEL_HEIGHT, m_conf.fg_color, m_conf.bg_color, m_emulator, SCALE_FACTOR));
    m_widgets.push_back(std::move(p1));
}

App::~App()
{

    if (m_audio_device > 0)
        SDL_CloseAudioDevice(m_audio_device);
    SDL_FreeWAV(m_wav_buffer); // Safe to call on nullptr
    SDL_Quit();
}

// FIXME: this should return EXIT_FAILURE when something goes wrong
int App::exec()
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

void App::handle_event()
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
        else if (m_event.type == SDL_MOUSEBUTTONDOWN)
        {
            fmt::print("Button down -> x={} y={}\n", m_event.motion.x, m_event.motion.y);
        }
    }
}

void App::render()
{
    set_renderer_color(m_renderer, m_conf.bg_color);
    SDL_RenderClear(m_renderer.get());
    for (widget_ptr const& widget : m_widgets)
    {
        widget->draw();
        SDL_SetRenderTarget(m_renderer.get(), NULL);
        SDL_RenderCopy(m_renderer.get(), widget->texture(), NULL, widget->rect());
    }
    SDL_RenderPresent(m_renderer.get());
}

void App::play_sound()
{
    if (m_audio_enabled)
    {
        sdl_checksuccess(SDL_QueueAudio(m_audio_device, m_wav_buffer, m_wavfile_length),
            "Failed to play sound");
        SDL_PauseAudioDevice(m_audio_device, false);
    }
}

std::vector<uint8_t> App::load_rom(std::filesystem::path rom_path)
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
