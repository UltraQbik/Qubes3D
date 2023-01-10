#include <iostream>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "vector.h"
#include "block_map.h"
#include "ray.h"


const unsigned int g_WIDTH = 1280;
const unsigned int g_HEIGHT = 720;

const unsigned int g_MAP_SIZE_X = 64;
const unsigned int g_MAP_SIZE_Y = 64;
const unsigned int g_MAP_SIZE_Z = 64;


std::array<std::array<std::array<uint16_t, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z> g_MAP;


int main() {
    // window setup
    sf::RenderWindow window(sf::VideoMode(g_WIDTH, g_HEIGHT), "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
    sf::Event ev;

    // create pixel array
    sf::Uint8* pixels = new sf::Uint8[g_WIDTH * g_HEIGHT * 4];

    // create buffer texture that we will update
    sf::Texture buffer;
    buffer.create(g_WIDTH, g_HEIGHT);

    // create the buffer sprite
    sf::Sprite bufferSprite(buffer);

    // generate a simple debug map (temporary)
    generate_debug_map(g_MAP);

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
        for (register int y = 0; y < g_HEIGHT; y++) {
            v = (float)y / g_HEIGHT * 2 - 1;
            for (register int x = 0; x < g_WIDTH; x++) {
                u = (float)x / g_WIDTH * 2 - 1;

                // TODO: ray casting function

                pixels[y * g_WIDTH * 4 + x * 4] = u * 127.5f + 127.5f;
                pixels[y * g_WIDTH * 4 + x * 4 + 1] = v * 127.5f + 127.5f;
                pixels[y * g_WIDTH * 4 + x * 4 + 2] = 0;
                pixels[y * g_WIDTH * 4 + x * 4 + 3] = 255;
            }
        }

        buffer.update(pixels); // update the buffer

        window.draw(bufferSprite); // blit image sprite buffer to the screen

        window.display(); // tell app that window is done drawing
    }

    // end of application
    return 0;
}