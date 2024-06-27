#pragma once

#include <SDL.h>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <yaml-cpp/yaml.h>

struct Config
{
public:
    static constexpr uint16_t default_width  = 640;
    static constexpr uint16_t default_height = 360;
    static constexpr uint16_t default_instructions_per_frame = 10;
    static constexpr SDL_Color default_fg_color = SDL_Color { 255, 255, 255, 255 };
    static constexpr SDL_Color default_bg_color = SDL_Color { 0, 0, 0, 255 };

    uint16_t window_width;
    uint16_t window_height;
    uint16_t instructions_per_frame;
    SDL_Color fg_color;
    SDL_Color bg_color;
    std::optional<std::filesystem::path> sound_file;
    std::optional<std::filesystem::path> font_file;

    Config()
        : window_width(default_width), window_height(default_height),
        instructions_per_frame(default_instructions_per_frame),
        fg_color(default_fg_color), bg_color(default_bg_color),
        sound_file(std::nullopt), font_file(std::nullopt)
    {}

    Config(YAML::Node const& yaml_conf)
    {
        window_width = val_from_conf(yaml_conf, "window_width", default_width);
        window_height = val_from_conf(yaml_conf, "window_height", default_height);

        instructions_per_frame = val_from_conf(yaml_conf, "instructions_per_frame", default_instructions_per_frame);

        fg_color = color_from_conf(yaml_conf, "foreground_color", default_fg_color);
        bg_color = color_from_conf(yaml_conf, "background_color", default_bg_color);

        if (YAML::Node val = yaml_conf["sound_file"])
            sound_file = val.as<std::string>();
        if (YAML::Node val = yaml_conf["font_file"])
            font_file = val.as<std::string>();
    }

private:
    template<typename T>
    T val_from_conf(YAML::Node const& yaml_conf, std::string const& key, T const& default_value)
    {
        if (yaml_conf[key])
            return  yaml_conf[key].as<T>();

        return default_value;
    }

    SDL_Color color_from_conf(YAML::Node const& yaml_conf, std::string const& key, SDL_Color default_color)
    {
        if (yaml_conf[key] && yaml_conf[key]["red"] && yaml_conf[key]["green"] && yaml_conf[key]["blue"])
        {
            uint8_t red = yaml_conf[key]["red"].as<uint8_t>(default_color.r);
            uint8_t green = yaml_conf[key]["green"].as<uint8_t>(default_color.g);
            uint8_t blue = yaml_conf[key]["blue"].as<uint8_t>(default_color.b);

            return SDL_Color { red, green, blue, 255 };
        }

        return default_color;
    }
};
