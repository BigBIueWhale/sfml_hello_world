#include <load_ariel_font/load_ariel_font.hpp>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <stdexcept>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    // Create a graphical text to display
    sf::Font font;
    {
        const auto ariel_font_bytes = load_ariel_from_memory();
        if (!font.loadFromMemory(ariel_font_bytes.data(), ariel_font_bytes.size())) {
            throw std::runtime_error("Failed to load font from file");
        }
    }
    sf::Text text("Hello SFML", font, 50);
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the string
        window.draw(text);
        // Update the window
        window.display();
    }
    return 0;
}
