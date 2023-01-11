#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "vector.h"
#include "blocks.h"


const unsigned int g_FRAME_CAP = 60;
const unsigned int g_MOUSE_SENSITIVITY = 64;
const unsigned int g_PLAYER_SPEED = 16;

const unsigned int g_RES = 80;
const unsigned int g_WIDTH = g_RES * 16;
const unsigned int g_HEIGHT = g_RES * 9;

const unsigned int g_MAP_SIZE_X = 32;
const unsigned int g_MAP_SIZE_Y = 32;
const unsigned int g_MAP_SIZE_Z = 32;


// framedelta
float framedelta = 0.0001f;

// camera position and rotation
Vec3<float> g_CAMP(16.0f, 16.0f, 18.0f);
Vec3<float> g_CAMR(0.0f, 0.0f, 0.0f);

// world map
std::array<std::array<std::array<Block, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z> g_MAP;

// window setup
sf::RenderWindow g_WIN(sf::VideoMode(g_WIDTH, g_HEIGHT), "Ray Tracing", sf::Style::Titlebar | sf::Style::Close);
sf::Event g_EV;


float cast_ray(Vec3<float>* pos, Vec3<float>* dir) {
    // basically DDA for 3D

    float d = 0.0f;
    Vec3<int> rp(floorf(pos->x), floorf(pos->y), floorf(pos->z));
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

    Block* blk;
    while (true) {
        blk = &g_MAP[rp.z][rp.y][rp.x];

        // check if block id is not air
        if (blk->id > 0)
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

Vec3<float> get_normal(Vec3<float>* pos) {
    Vec3<float> normal;

    normal.x = (g_MAP[(int)pos->z][(int)pos->y][(int)(pos->x - 1e-5f)].tags & 1) - (g_MAP[(int)pos->z][(int)pos->y][(int)(pos->x + 1e-5f)].tags & 1);
    normal.y = (g_MAP[(int)pos->z][(int)(pos->y - 1e-5f)][(int)pos->x].tags & 1) - (g_MAP[(int)pos->z][(int)(pos->y + 1e-5f)][(int)pos->x].tags & 1);
    normal.z = (g_MAP[(int)(pos->z - 1e-5f)][(int)pos->y][(int)pos->x].tags & 1) - (g_MAP[(int)(pos->z + 1e-5f)][(int)pos->y][(int)pos->x].tags & 1);

    return normal;
}

Vec3<char> calculate_pixel(Vec2<float>* coord) {
    Vec3<float> dir(coord->x, 1, coord->y);
    dir = dir.norm();
    dir = rotate_z(rotate_x(dir, g_CAMR.x), g_CAMR.z);

    float dist = cast_ray(&g_CAMP, &dir);
    Vec3<float> hit_point = dir * dist + g_CAMP;

    Vec3<float> normal = get_normal(&hit_point);

    return Vec3<char>((char)(normal.x * 127.5f + 127.5f), (char)(normal.y * 127.5f + 127.5f), (char)(normal.z * 127.5f + 127.5f));
}

void generate_debug_map() {
    // This will generate just a 3D grid of blocks with random id's

    for (int i = 0; i < g_MAP_SIZE_Z; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                {
                    g_MAP[i][j][k].id = rand() % 256;
                    g_MAP[i][j][k].tags = 1;
                }
}

void generate_flatworld_map() {
    // This will generate a simple flatworld, with ground level starting at g_MAP_SIZE_Z / 2 - 1

    for (int i = 0; i < g_MAP_SIZE_Z / 2; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
            {
                g_MAP[i][j][k].id = 1;
                g_MAP[i][j][k].tags = 1;
            }
}

void player_controls() {
    // mouse controls
    Vec2<float> mouse_movement((float)sf::Mouse::getPosition(g_WIN).x / g_WIDTH - .5f, (float)sf::Mouse::getPosition(g_WIN).y / g_HEIGHT - .5f);    // gets the amount the mouse moved
    sf::Mouse::setPosition(sf::Vector2i(g_WIDTH / 2, g_HEIGHT / 2), g_WIN);                                                                         // snaps the mouse back to the middle of the screen

    // move the camera according to mouse movement
    g_CAMR.x = std::fmodf(g_CAMR.x - mouse_movement.y * g_MOUSE_SENSITIVITY * framedelta, 3.14159265);
    g_CAMR.z = std::fmodf(g_CAMR.z - mouse_movement.x * g_MOUSE_SENSITIVITY * framedelta, 3.14159265);

    // std::cout << g_CAMR.x << " | " << g_CAMR.y << " | " << g_CAMR.z << "\n";

    // key presses
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        g_CAMP.x += std::cosf(g_CAMR.z + 1.5707963) * g_PLAYER_SPEED * framedelta;
        g_CAMP.y += std::sinf(g_CAMR.z + 1.5707963) * g_PLAYER_SPEED * framedelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        g_CAMP.x -= std::cosf(g_CAMR.z + 1.5707963) * g_PLAYER_SPEED * framedelta;
        g_CAMP.y -= std::sinf(g_CAMR.z + 1.5707963) * g_PLAYER_SPEED * framedelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        g_CAMP.x -= std::cosf(g_CAMR.z) * g_PLAYER_SPEED * framedelta;
        g_CAMP.y -= std::sinf(g_CAMR.z) * g_PLAYER_SPEED * framedelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        g_CAMP.x += std::cosf(g_CAMR.z) * g_PLAYER_SPEED * framedelta;
        g_CAMP.y += std::sinf(g_CAMR.z) * g_PLAYER_SPEED * framedelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        g_CAMP.z += 8 * framedelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        g_CAMP.z -= 8 * framedelta;
    }
}

void calculate_pixel_range(sf::Uint8* pixels, int begin, int end) {
    Vec2<float> coord;
    Vec3<char> color;
    for (register int y = begin; y < end; y++) {
        coord.y = -((float)y / g_HEIGHT * 2 - 1);
        for (register int x = 0; x < g_WIDTH; x++) {
            coord.x = (float)x / g_WIDTH * 2 - 1;

            color = calculate_pixel(&coord);

            pixels[y * g_WIDTH * 4 + x * 4] = color.x;
            pixels[y * g_WIDTH * 4 + x * 4 + 1] = color.y;
            pixels[y * g_WIDTH * 4 + x * 4 + 2] = color.z;
            pixels[y * g_WIDTH * 4 + x * 4 + 3] = 255;
        }
    }
}


int main() {
    // create pixel array
    sf::Uint8* pixels = new sf::Uint8[g_WIDTH * g_HEIGHT * 4];

    // create buffer texture that we will update
    sf::Texture buffer;
    buffer.create(g_WIDTH, g_HEIGHT);

    // create the buffer sprite
    sf::Sprite bufferSprite(buffer);

    // timers for calculating the framedelta
    std::chrono::steady_clock::time_point t1, t2;

    // generate a simple debug map (temporary)
    generate_flatworld_map();

    // delay between frames
    float delay = 0;

    // game loop
    while (g_WIN.isOpen()) {
        // timer 1
        t1 = std::chrono::high_resolution_clock::now();

        // event polling
        while (g_WIN.pollEvent(g_EV)) {
            switch (g_EV.type) {
            case sf::Event::Closed:
                g_WIN.close();
                break;
            case sf::Event::KeyPressed:
                if (g_EV.key.code == sf::Keyboard::Escape)
                    g_WIN.close();
                break;
            }
        }

        // player controls
        player_controls();

        // game render
        g_WIN.clear(sf::Color(255, 255, 255, 255)); // clear the old frame

        // render frame
        std::thread worker1(calculate_pixel_range, pixels, 0, (int)(g_HEIGHT * 1 / 6));
        std::thread worker2(calculate_pixel_range, pixels, (int)(g_HEIGHT * 1 / 6), (int)(g_HEIGHT * 2 / 6));
        std::thread worker3(calculate_pixel_range, pixels, (int)(g_HEIGHT * 2 / 6), (int)(g_HEIGHT * 3 / 6));
        std::thread worker4(calculate_pixel_range, pixels, (int)(g_HEIGHT * 3 / 6), (int)(g_HEIGHT * 4 / 6));
        std::thread worker5(calculate_pixel_range, pixels, (int)(g_HEIGHT * 4 / 6), (int)(g_HEIGHT * 5 / 6));
        std::thread worker6(calculate_pixel_range, pixels, (int)(g_HEIGHT * 5 / 6), (int)(g_HEIGHT));

        worker1.join();
        worker2.join();
        worker3.join();
        worker4.join();
        worker5.join();
        worker6.join();

        buffer.update(pixels); // update the buffer

        g_WIN.draw(bufferSprite); // blit image sprite buffer to the screen

        g_WIN.display(); // tell app that window is done drawing

        // timer 2
        t2 = std::chrono::high_resolution_clock::now();

        // calculate framedelta
        framedelta = (t2 - t1).count() / 1e9f;

        // std::cout << "FPS: " << 1 / framedelta << "\n";

        // wait some time between frames
        delay = 1000.0f / g_FRAME_CAP - framedelta * 1000;
        if (delay > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)delay));
    }

    // end of application
    delete[] pixels;
    return 0;
}