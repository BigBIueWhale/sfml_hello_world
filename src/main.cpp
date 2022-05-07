#include <load_ariel_font/load_ariel_font.hpp>
#include "Pixel.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>
#include <memory>
#include <vector>
#include <array>
#include <initializer_list>
#include <optional>
#include <chrono>
#include <random>


int main()
{
    //const std::vector<sf::VideoMode>& fullscreen_video_modes = sf::VideoMode::getFullscreenModes();

    // Create the main window
    sf::RenderWindow window{
        sf::VideoMode{ 800, 600, 32 }, //fullscreen_video_modes.at(0),
        "SFML window",
        sf::Style::Default //sf::Style::Fullscreen
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

    std::optional<std::chrono::steady_clock::time_point> most_recent_time_measurement;
    long long counter_frames = 0;
    std::string fps_text;

    const auto random_engine_ptr = std::make_unique<std::mt19937_64>();
    auto& random_engine = *random_engine_ptr;

    std::optional<sf::Vector2i> most_recent_left_mouse_button_down_coordinates;

    sf::Texture image_texture;
    {
        sf::Image image_from_file;
        if (!image_from_file.loadFromFile(R"(C:\Users\rzyro\source\github\sfml_hello_world\resources\binary_search_divide_performance.png)")) {
            throw std::runtime_error("Failed to load image file from disk");
        }
        image_texture.loadFromImage(image_from_file);
    }
    const sf::Sprite image_sprite{ image_texture };

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

        const sf::Vector2u window_size = window.getSize();

        // Draw individual pixels onto the screen from a CPU-controlled array.
        {
            sf::Texture pixels_texture;
            {
                sf::Image pixels_image;
                {
                    std::vector<Pixel> pixels_rgba(
                        static_cast<std::ptrdiff_t>(window_size.x) * window_size.y,
                        Pixel{ 0, 0, 0, 0 }
                    );
                    auto get_rand_pixel = [&random_engine]() -> Pixel
                    {
                        const std::uniform_int_distribution<int> random_8bit{ 0, 255 };
                        return Pixel{
                            static_cast<std::uint8_t>(random_8bit(random_engine)),
                            static_cast<std::uint8_t>(random_8bit(random_engine)),
                            static_cast<std::uint8_t>(random_8bit(random_engine)),
                            40
                        };
                    };

                    Pixel pixel_val = get_rand_pixel();

                    for (std::ptrdiff_t idx_row = 0; idx_row < static_cast<std::ptrdiff_t>(window_size.y); ++idx_row) {
                        if (random_engine() % 30 == 0) {
                            pixel_val = get_rand_pixel();
                        }
                        for (std::ptrdiff_t idx_column = 0; idx_column < static_cast<std::ptrdiff_t>(window_size.x); ++idx_column) {
                            const auto idx_pixel = idx_row * static_cast<std::ptrdiff_t>(window_size.x) + idx_column;
                            auto& pixel_ref = pixels_rgba[idx_pixel];
                            pixel_ref = pixel_val;
                        }
                    }
                    pixels_image.create(window_size.x, window_size.y, view_as_bytes(pixels_rgba).data());
                }
                pixels_texture.loadFromImage(pixels_image);
            }
            const sf::Sprite pixels_sprite{ pixels_texture };
            window.draw(pixels_sprite);
        }

        // Calculate the FPS
        {
            const auto time_now = std::chrono::steady_clock::now();
            ++counter_frames;
            if (most_recent_time_measurement.has_value())
            {
                const auto time_passed = time_now - most_recent_time_measurement.value();
                if (time_passed >= std::chrono::milliseconds{ 700 }) {
                    most_recent_time_measurement = time_now;
                    const std::chrono::nanoseconds time_passed_nanoseconds = time_passed;
                    const double time_passed_seconds = time_passed_nanoseconds.count() / 1e9;
                    {
                        std::ostringstream string_builder;
                        string_builder << "FPS: " << counter_frames / time_passed_seconds;
                        fps_text = string_builder.str();
                    }
                    counter_frames = 0;
                }
            }
            else {
                most_recent_time_measurement = time_now;
            }
        }

        // Draw the FPS text on the screen
        {
            const sf::View dragged_view = window.getView();
            window.setView(sf::View{
                sf::FloatRect{
                    0,
                    0,
                    static_cast<float>(window.getSize().x),
                    static_cast<float>(window.getSize().y)
                }
                });
            const sf::Text text{ fps_text, font, 12 };
            // Draw the string
            window.draw(text);
            window.setView(dragged_view);
        }

        // Draw lines on the screen using the GPU
        {
            auto get_random_vertex = [&random_engine, window_size = window.getSize()]() -> sf::Vertex
            {
                const sf::Color line_color{ 0xff, 0x00, 0x00 };
                const std::uniform_real_distribution<float> rand_x{ 0.0f, static_cast<float>(window_size.x - 1) };
                const std::uniform_real_distribution<float> rand_y{ 0.0f, static_cast<float>(window_size.y - 1) };
                return sf::Vertex{
                    sf::Vector2f{ rand_x(random_engine), rand_y(random_engine) },
                    line_color
                };
            };
            const auto line = std::make_unique<std::array<sf::Vertex, 69>>();
            for (sf::Vertex& vertex : *line) {
                vertex = get_random_vertex();
            }
            window.draw(line->data(), line->size(), sf::Lines);
        }

        // Update the window
        window.display();
    }
    return 0;
}
