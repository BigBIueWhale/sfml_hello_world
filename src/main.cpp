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

    sf::Vector2i most_recent_window_size{
        window.getSize()
    };
    std::optional<sf::Vector2i> most_recent_left_mouse_button_down_coordinates;

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
                    const sf::View& previous_view{
                        window.getView()
                    };
                    const sf::Vector2f new_window_size{
                        static_cast<float>(event.size.width),
                        static_cast<float>(event.size.height)
                    };
                    const sf::Vector2f change_ratio{
                        new_window_size.x / static_cast<float>(most_recent_window_size.x),
                        new_window_size.y / static_cast<float>(most_recent_window_size.y)
                    };
                    const sf::View new_view{
                        previous_view.getCenter(),
                        sf::Vector2f{
                            previous_view.getSize().x * change_ratio.x,
                            previous_view.getSize().y * change_ratio.y
                        }
                    };
                    window.setView(new_view);
                    most_recent_window_size = sf::Vector2i{
                        window.getSize()
                    };
                    break;
                }
                case sf::Event::EventType::MouseMoved:
                {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        const sf::Vector2i mouse_pos{
                            event.mouseMove.x,
                            event.mouseMove.y
                        };
                        if (most_recent_left_mouse_button_down_coordinates.has_value()) {
                            const sf::Vector2i change_in_pixels{
                                mouse_pos - most_recent_left_mouse_button_down_coordinates.value()
                            };
                            const sf::Vector2f proportion_change{
                                static_cast<float>(change_in_pixels.x) / window.getSize().x,
                                static_cast<float>(change_in_pixels.y) / window.getSize().y
                            };
                            const sf::Vector2f size_of_world{
                                window.getView().getSize()
                            };
                            const sf::Vector2f change_in_world_coordinates{
                                proportion_change.x * size_of_world.x,
                                proportion_change.y * size_of_world.y
                            };
                            sf::Vector2f center_of_world{
                                window.getView().getCenter()
                            };
                            center_of_world.x -= change_in_world_coordinates.x;
                            center_of_world.y -= change_in_world_coordinates.y;
                            window.setView(sf::View{ center_of_world, size_of_world });
                        }
                        most_recent_left_mouse_button_down_coordinates = mouse_pos;
                    }
                    else {
                        most_recent_left_mouse_button_down_coordinates = std::nullopt;
                    }
                    break;
                }
                case sf::Event::EventType::MouseWheelScrolled:
                {
                    sf::Vector2f size_of_world{
                        window.getView().getSize()
                    };
                    const float factor = std::pow(1.1f, -event.mouseWheelScroll.delta);
                    size_of_world.x *= factor;
                    size_of_world.y *= factor;
                    window.setView(sf::View{ window.getView().getCenter(), size_of_world });
                    break;
                }
            }
        };

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
