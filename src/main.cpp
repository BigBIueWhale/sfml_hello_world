#include <load_ariel_font/load_ariel_font.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <vector>
#include <array>
#include <chrono>
#include <optional>
#include <cmath>

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

    sf::Vector2f center_of_world{ window.getView().getCenter() };
    sf::Vector2f size_of_world{ window.getSize() };
    std::optional<sf::Vector2f> mouse_most_recent_time_saw_moving;

    
    sf::Texture image_texture;
    {
        sf::Image image_from_file;
        if (!image_from_file.loadFromFile(R"(C:\Users\rzyro\source\github\sfml_hello_world\resources\binary_search_divide_performance.png)")) {
            throw std::runtime_error("Failed to load image file from disk");
        }
        image_texture.loadFromImage(image_from_file);
    }
    sf::Sprite image_sprite{ image_texture };

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::EventType::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::EventType::Resized:
                {
                    break;
                }
                case sf::Event::EventType::MouseMoved:
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        const sf::Vector2f mouse_pos =
                            window.mapPixelToCoords(sf::Vector2i{ event.mouseMove.x, event.mouseMove.y });
                        if (mouse_most_recent_time_saw_moving.has_value()) {
                            const sf::Vector2f change_since_last_time{
                                mouse_most_recent_time_saw_moving.value() - mouse_pos
                            };
                            mouse_most_recent_time_saw_moving = std::nullopt;
                            center_of_world.x += change_since_last_time.x;
                            center_of_world.y += change_since_last_time.y;
                            window.setView(sf::View{ center_of_world, size_of_world });
                        }
                        mouse_most_recent_time_saw_moving = mouse_pos;
                    }
                    else {
                        mouse_most_recent_time_saw_moving = std::nullopt;
                    }
                    break;
                }
                case sf::Event::EventType::MouseWheelScrolled:
                {
                    const float factor = std::pow(1.1f, -event.mouseWheelScroll.delta);
                    size_of_world.x *= factor;
                    size_of_world.y *= factor;
                    break;
                }
            }
        };

        window.setView(sf::View{ center_of_world, size_of_world });

        // Clear screen
        window.clear();

        window.draw(image_sprite);

        sf::Text text{ "Hello from Ronen", font, 12 };
        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }
    return 0;
}
