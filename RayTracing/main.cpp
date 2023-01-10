#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "vector.h"


const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;





int main() {
    // window setup
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
    sf::Event ev;

    // create pixel array
    sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

    // create buffer texture that we will update
    sf::Texture buffer;
    buffer.create(WIDTH, HEIGHT);

    // create the buffer sprite
    sf::Sprite bufferSprite(buffer);

    // game loop
    while (window.isOpen()) {
        // event polling
        while (window.pollEvent(ev)) {
            switch (ev.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            }
        }

        // game render
        window.clear(sf::Color(255, 255, 255, 255)); // clear the old frame

        // fill the fixels
        float u, v;
        for (register int y = 0; y < HEIGHT; y++) {
            v = (float)y / HEIGHT * 2 - 1;
            for (register int x = 0; x < WIDTH; x++) {
                u = (float)x / WIDTH * 2 - 1;

                pixels[y * WIDTH * 4 + x * 4] = u * 127.5f + 127.5f;
                pixels[y * WIDTH * 4 + x * 4 + 1] = v * 127.5f + 127.5f;
                pixels[y * WIDTH * 4 + x * 4 + 2] = 0;
                pixels[y * WIDTH * 4 + x * 4 + 3] = 255;
            }
        }

        buffer.update(pixels); // update the buffer

        window.draw(bufferSprite); // blit image sprite buffer to the screen

        window.display(); // tell app that window is done drawing
    }

    // end of application
    return 0;
}