#pragma once

#include "pixel.hpp"

#include <SFML/Graphics.hpp>

#include <span>

class sfml_window_helpers {
public:
    sfml_window_helpers() = delete;
    static void draw_pixels_onto_window(
            const std::span<const pixel> row_major_2d_rgba,
            const sf::Vector2u& arr_dimensions,
            sf::RenderWindow& window);
};
