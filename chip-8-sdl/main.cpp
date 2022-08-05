#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <vector>

#include "argparser.h"
#include "chip8.h"

std::vector<uint8_t> load_rom(std::filesystem::path rom_path);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage : chip-8-sdl [OPTION] rom_file\n\n"
            "Options:\n"
            "    -f config_file:\n"
            "        path of the YAML configuration file to apply\n";
        exit(EXIT_FAILURE);
    }

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 320, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    uint32_t wav_length = 0;
    uint8_t* wav_buffer = nullptr;
    SDL_AudioSpec wav_spec;
    if (SDL_LoadWAV("C:/Users/potierf/Desktop/Chip-8/chip-8-sdl/res/beep-02.wav", &wav_spec, &wav_buffer, &wav_length))
        std::cerr << "Unable to load the beep .wav file\n";

    SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(nullptr, 0, &wav_spec, nullptr, 0);

    std::vector<uint8_t> program = load_rom(std::filesystem::path(argv[1]));
    chip8 emulator = chip8(program.data(), program.size());

    SDL_Rect rect;
    rect.w = 10; rect.h = 10;
    SDL_RenderFillRect(renderer, &rect);
    bool quit = false;
    while (!quit)
    {
        uint64_t frame_start = SDL_GetPerformanceCounter();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // set keys
            if (event.type == SDL_QUIT)
                    quit = true;
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1: emulator.key_pressed(1); break;
                    case SDLK_2: emulator.key_pressed(2); break;
                    case SDLK_3: emulator.key_pressed(3); break;
                    case SDLK_4: emulator.key_pressed(12); break;
                    case SDLK_a: emulator.key_pressed(4); break;
                    case SDLK_z: emulator.key_pressed(5); break;
                    case SDLK_e: emulator.key_pressed(6); break;
                    case SDLK_r: emulator.key_pressed(13); break;
                    case SDLK_q: emulator.key_pressed(7); break;
                    case SDLK_s: emulator.key_pressed(8); break;
                    case SDLK_d: emulator.key_pressed(9); break;
                    case SDLK_f: emulator.key_pressed(14); break;
                    case SDLK_w: emulator.key_pressed(10); break;
                    case SDLK_x: emulator.key_pressed(0); break;
                    case SDLK_c: emulator.key_pressed(11); break;
                    case SDLK_v: emulator.key_pressed(15); break;
                    case SDLK_ESCAPE: quit = true; break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1: emulator.key_released(1); break;
                    case SDLK_2: emulator.key_released(2); break;
                    case SDLK_3: emulator.key_released(3); break;
                    case SDLK_4: emulator.key_released(12); break;
                    case SDLK_a: emulator.key_released(4); break;
                    case SDLK_z: emulator.key_released(5); break;
                    case SDLK_e: emulator.key_released(6); break;
                    case SDLK_r: emulator.key_released(13); break;
                    case SDLK_q: emulator.key_released(7); break;
                    case SDLK_s: emulator.key_released(8); break;
                    case SDLK_d: emulator.key_released(9); break;
                    case SDLK_f: emulator.key_released(14); break;
                    case SDLK_w: emulator.key_released(10); break;
                    case SDLK_x: emulator.key_released(0); break;
                    case SDLK_c: emulator.key_released(11); break;
                    case SDLK_v: emulator.key_released(15); break;
                }
            }
        }

        // emulate
        emulator.tick(10);
        // draw
        std::array<uint8_t, chip8::vram_size> const& emulator_vram = emulator.get_vram();
        if (emulator.vram_dirty)
        {
            rect.x = 0; rect.y = 0;
            SDL_RenderClear(renderer);
            for (std::size_t y = 0; y < chip8::screen_height; y++)
            {
                rect.y = y * 10;
                for (std::size_t x = 0; x < chip8::screen_width; x++)
                {
                    rect.x = x * 10;
                    uint8_t pixel = emulator_vram[x + y * chip8::screen_width];
                    if (pixel)
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    else
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                    SDL_RenderFillRect(renderer, &rect);
                }
            }
            SDL_RenderPresent(renderer);
        }

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
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    if (wav_buffer)
        SDL_FreeWAV(wav_buffer);
    if (audio_device)
        SDL_CloseAudioDevice(audio_device);
    SDL_Quit();

    return 0;
}

std::vector<uint8_t> load_rom(std::filesystem::path rom_path)
{
    std::ifstream rom_file(rom_path.string(), std::ios::binary);
    std::streampos rom_size;

    rom_file.seekg(0, std::ios::end);
    rom_size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom_content(rom_size);
    rom_file.read(reinterpret_cast<char*>(rom_content.data()), rom_size);

    return rom_content;
}
