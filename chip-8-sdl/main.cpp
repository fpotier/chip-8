#include <cmath>
#include <cstdint>
#include <cxxopts.hpp>
#include <filesystem>
#include <SDL.h>
#include <yaml-cpp/yaml.h>

#include "argparser.h"
#include "app.h"
#include "chip8.h"
#include "config.h"

int main(int argc, char** argv)
{
    cxxopts::Options options("Chip 8 Emulator", "Simple Chip 8 Emulator written with the SDL2 library");
    options.add_options()
        ("rom", "Chip 8 ROM to load and run", cxxopts::value<std::string>())
        ("f,file", "YAML configuration file to apply", cxxopts::value<std::string>())
        ("h,help", "Display help and exit");
    options.parse_positional({ "rom" });
    options.positional_help("ROM");

    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << '\n';
        exit(EXIT_SUCCESS);
    }
    if (!result.count("rom"))
    {
        std::cerr << options.help() << '\n';
        exit(EXIT_FAILURE);
    }

    config conf;
    if (result.count("file"))
    {
        YAML::Node config_yaml = YAML::LoadFile(result["file"].as<std::string>());
        conf = config(config_yaml);
    }

    std::vector<uint8_t> rom_content = app::load_rom(result["rom"].as<std::string>());
    app chip8_emulator(conf, rom_content.data(), rom_content.size());

    return chip8_emulator.exec();
}

    /*
    uint32_t wav_length = 0;
    uint8_t* wav_buffer = nullptr;
    SDL_AudioSpec wav_spec;
    if (SDL_LoadWAV("C:/Users/potierf/Desktop/Chip-8/chip-8-sdl/res/beep-02.wav", &wav_spec, &wav_buffer, &wav_length))
        std::cerr << "Unable to load the beep .wav file\n";

    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(nullptr, 0, &wav_spec, nullptr, 0);

        if (audio_device && wav_buffer
            && emulator.get_sound_timer() == 1)
        {
            SDL_QueueAudio(audio_device, wav_buffer, wav_length);
            SDL_PauseAudioDevice(audio_device, false);
        }

        uint64_t frame_end = SDL_GetPerformanceCounter();
        float elapsed_ms = (frame_end - frame_start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;
        float to_wait = std::floor(16.6f - elapsed_ms);
        if (to_wait > 0)
            SDL_Delay(to_wait);
    */
