#include <cmath>
#include <cstdint>
#include <cxxopts.hpp>
#include <filesystem>
#include <SDL.h>
#include <yaml-cpp/yaml.h>

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