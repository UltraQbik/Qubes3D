#include <iostream>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "vector.h"
#include "blocks.h"


const unsigned int g_RES = 40;
const unsigned int g_WIDTH = g_RES * 16;
const unsigned int g_HEIGHT = g_RES * 9;

const unsigned int g_MAP_SIZE_X = 64;
const unsigned int g_MAP_SIZE_Y = 64;
const unsigned int g_MAP_SIZE_Z = 64;


Vec3<float> g_CAMP(32.0f, 32.0f, 36.0f);
Vec3<float> g_CAMR(0.0f, 0.0f, 0.0f);
std::array<std::array<std::array<Block, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z> g_MAP;


float cast_ray(Vec3<float>* pos, Vec3<float>* dir) {
    float d = 0.0f;
    Vec3<int64_t> rp(floorf(pos->x), floorf(pos->y), floorf(pos->z));
    Vec3<float> unit(fabsf(1 / dir->x), fabsf(1 / dir->y), fabsf(1 / dir->z));
    Vec3<float> step;
    Vec3<float> len;

    if (dir->x > 0) {
        step.x = 1.0f;
        len.x = (rp.x - pos->x + 1) * unit.x;
    }
    else {
        step.x = -1.0f;
        len.x = (pos->x - rp.x) * unit.x;
    }
    if (dir->y > 0) {
        step.y = 1.0f;
        len.y = (rp.y - pos->y + 1) * unit.y;
    }
    else {
        step.y = -1.0f;
        len.y = (pos->y - rp.y) * unit.y;
    }
    if (dir->z > 0) {
        step.z = 1.0f;
        len.z = (rp.z - pos->z + 1) * unit.z;
    }
    else {
        step.z = -1.0f;
        len.z = (pos->z - rp.z) * unit.z;
    }

    // quick NAN fix when multiplying by float inf
    if (dir->x == 0) len.x = INFINITY;
    if (dir->y == 0) len.y = INFINITY;
    if (dir->z == 0) len.z = INFINITY;

    if (rp.x < 0 || rp.x > g_MAP_SIZE_X - 1 || rp.y < 0 || rp.y > g_MAP_SIZE_Y - 1 || rp.z < 0 || rp.z > g_MAP_SIZE_Z - 1)
        return d;

    Block blk;
    while (true) {
        blk = g_MAP[(int)rp.z][(int)rp.y][(int)rp.x];

        // check if block id is not air
        if (blk.id != 0)
            return d;

        if (len.x < len.y && len.x < len.z) {
            rp.x += step.x;
            d = len.x;
            len.x += unit.x;
            if (rp.x < 0 || rp.x > g_MAP_SIZE_X - 1)
                return d;
        }
        else if (len.y < len.z) {
            rp.y += step.y;
            d = len.y;
            len.y += unit.y;
            if (rp.y < 0 || rp.y > g_MAP_SIZE_Y - 1)
                return d;
        }
        else {
            rp.z += step.z;
            d = len.z;
            len.z += unit.z;
            if (rp.z < 0 || rp.z > g_MAP_SIZE_Z - 1)
                return d;
        }
    }
}


Vec3<char> calculate_pixel(Vec2<float>* coord) {
    Vec3<float> dir(coord->x, 1, coord->y);
    dir = dir.norm();

    float dist = cast_ray(&g_CAMP, &dir);

    float brightness = 1024 / dist;
    if (brightness > 255)
        return Vec3<char>(255, 255, 255);
    return Vec3<char>((char)brightness, (char)brightness, (char)brightness);
    
}


void generate_debug_map() {
    // This will generate just a 3D grid of blocks with random id's

    for (int i = 0; i < g_MAP_SIZE_Z; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    g_MAP[i][j][k] = rand() % 256;
}

void generate_flatworld_map() {
    // This will generate a simple flatworld, with ground level starting at g_MAP_SIZE_Z / 2 - 1

    for (int i = 0; i < g_MAP_SIZE_Z / 2; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                g_MAP[i][j][k] = 1;
}


int main() {
    // window setup
    sf::RenderWindow window(sf::VideoMode(g_WIDTH, g_HEIGHT), "Ray Tracing", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    sf::Event ev;

    // create pixel array
    sf::Uint8* pixels = new sf::Uint8[g_WIDTH * g_HEIGHT * 4];

    // create buffer texture that we will update
    sf::Texture buffer;
    buffer.create(g_WIDTH, g_HEIGHT);

    // create the buffer sprite
    sf::Sprite bufferSprite(buffer);

    // generate a simple debug map (temporary)
    generate_debug_map();

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
        Vec2<float> coord;
        Vec3<char> color;
        for (register int y = 0; y < g_HEIGHT; y++) {
            coord.y = (float)y / g_HEIGHT * 2 - 1;
            for (register int x = 0; x < g_WIDTH; x++) {
                coord.x = (float)x / g_WIDTH * 2 - 1;

                color = calculate_pixel(&coord);

                pixels[y * g_WIDTH * 4 + x * 4] = color.x;
                pixels[y * g_WIDTH * 4 + x * 4 + 1] = color.y;
                pixels[y * g_WIDTH * 4 + x * 4 + 2] = color.z;
                pixels[y * g_WIDTH * 4 + x * 4 + 3] = 255;
            }
        }

        // more the camera
        g_CAMP.x = std::cos(clock() / 2048.0f) * 16.0f + 32.0f;
        g_CAMP.z = std::sin(clock() / 2048.0f) * 16.0f + 32.0f;

        buffer.update(pixels); // update the buffer

        window.draw(bufferSprite); // blit image sprite buffer to the screen

        window.display(); // tell app that window is done drawing
    }

    // end of application
    delete[] pixels;
    return 0;
}