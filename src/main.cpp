#include "sfml_window_helpers.hpp"

#include <load_ariel_font/load_ariel_font.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <vector>
#include <array>
#include <chrono>

int main()
{
    // Create the main window
    sf::RenderWindow window{
        sf::VideoMode{ 800, 600, 32 },
        "SFML window",
        sf::Style::Default
    };

    // Create a graphical text to display
    sf::Font font;
    {
        const auto ariel_font_bytes = load_ariel_from_memory();
        if (!font.loadFromMemory(ariel_font_bytes.data(), ariel_font_bytes.size())) {
            throw std::runtime_error("Failed to load font from file");
        }
    }

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                // Required to stop SFML's default behaviour of
                // using the original window size coordinates forever.
                case sf::Event::Resized:
                {
                    sf::FloatRect view(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View{ view });
                    break;
                }
            }
        };

        // Clear screen
        window.clear();

        auto time_now = std::chrono::steady_clock::now();
        static std::chrono::steady_clock::time_point previous_time;

        auto time_difference = time_now - previous_time;

        auto time_seconds = time_difference.count() / 1e9;

        auto fps = 1.0 / time_seconds;

        previous_time = time_now;

        std::array<pixel, 1650> pixels;
        for (int i = 0; i < 1650; i = i + 1) {
            pixels[i] = pixel(i / 1650.0 * 255, fps, 255);
        }

        sfml_window_helpers::draw_pixels_onto_window(pixels, sf::Vector2u{ 30, 55 }, window);
       
        sf::Text text{ std::to_string(fps), font, 100 };
        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }
    return 0;
}
