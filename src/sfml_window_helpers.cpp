#include "sfml_window_helpers.hpp"

#include <cstddef>
#include <new>
#include <sstream>
#include <stdexcept>

void sfml_window_helpers::draw_pixels_onto_window(
    const std::span<const pixel> row_major_2d_rgba,
    const sf::Vector2u& arr_dimensions,
    sf::RenderWindow& window)
{
    const auto expected_arr_sz = static_cast<std::ptrdiff_t>(arr_dimensions.x)
        * static_cast<std::ptrdiff_t>(arr_dimensions.y);
    if (static_cast<std::ptrdiff_t>(row_major_2d_rgba.size()) != expected_arr_sz
        || row_major_2d_rgba.empty()) {
        std::ostringstream err_msg;
        err_msg << "Invalid size of pixel array to draw on screen."
            << " Given size: " << row_major_2d_rgba.size()
            << " Expected size: " << expected_arr_sz
            << " a size of 0 is not allowed.";
        throw std::runtime_error(err_msg.str());
    }
    const sf::Vector2u window_sz = window.getSize();
    sf::Texture texture;
    {
        sf::Image image;
        static_assert(sizeof(pixel) == 4, "Assuming that pixel is just a uint32_t in memory");
        const auto raw_data_ptr = std::launder(reinterpret_cast<const sf::Uint8*>(row_major_2d_rgba.data()));
        image.create(arr_dimensions.x, arr_dimensions.y, raw_data_ptr);
        if (!texture.loadFromImage(image)) {
            throw std::runtime_error("Failed to load texture from file");
        }
    }
    sf::Sprite sprite{ texture };
    const auto x_scale = static_cast<float>(window_sz.x) / static_cast<float>(arr_dimensions.x);
    const auto y_scale = static_cast<float>(window_sz.y) / static_cast<float>(arr_dimensions.y);
    sprite.setScale(sf::Vector2f{ x_scale, y_scale });
    window.draw(sprite);
}
