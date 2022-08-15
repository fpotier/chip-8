#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <yaml-cpp/yaml.h>

struct config
{
public:
    static constexpr uint16_t default_width  = 640;
    static constexpr uint16_t default_height = 320;
    static constexpr SDL_Color default_fg_color = SDL_Color{ 255, 255, 255, 255 };
    static constexpr SDL_Color default_bg_color = SDL_Color{ 0, 0, 0, 255 };

    uint16_t window_width;
    uint16_t window_height;
    SDL_Color fg_color;
    SDL_Color bg_color;
    std::optional<std::filesystem::path> sound_file;

    config()
        : window_width(default_width), window_height(default_height),
        sound_file(std::nullopt)
    {}

    config(YAML::Node const& yaml_conf)
    {
        window_width = val_from_conf(yaml_conf, "window_width", default_width);
        window_height = val_from_conf(yaml_conf, "window_height", default_height);

        if (YAML::Node val = yaml_conf["sound_file"])
            sound_file = val.as<std::string>();
    }

private:
    template<typename T>
    T val_from_conf(YAML::Node const& yaml_conf, std::string const& key, T const& default_value)
    {
        if (yaml_conf[key])
            return  yaml_conf[key].as<T>();

        return default_value;
    }
};
