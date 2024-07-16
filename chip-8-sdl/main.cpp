#include <SDL.h>
#include <cxxopts.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <iostream>
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

    Config conf;
    if (result.count("file"))
    {
        fmt::print("Loading config {}\n", result["file"].as<std::string>());
        YAML::Node config_yaml = YAML::LoadFile(result["file"].as<std::string>());
        conf = Config(config_yaml);
    }

    std::string rom_path = result["rom"].as<std::string>();
    std::vector<uint8_t> rom_content = App::load_rom(rom_path);
    App chip8_emulator(conf, rom_path, rom_content);

    return chip8_emulator.exec();
}
