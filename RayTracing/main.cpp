#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
// #include <SFML/Audio.hpp>
// #include <SFML/Network.hpp>

#include "configuration.h"
#include "camera.h"
#include "window.h"
#include "vector.h"
#include "world.h"
#include "noise.h"


// window
Window g_WIN(Vec2<uint16_t>(1280, 720), 75);

// player camera
Camera g_CAMERA(g_WIN.GetWindow(), 0.8f, Vec3<float>(g_MAP_SIZE_X / 2.f, g_MAP_SIZE_Y / 2.f, g_MAP_SIZE_Z / 2.f));

// world (just a chunk for now)
Chunk g_MAP(0);

// framedelta
float framedelta = 0.0001f;

// ray hit data
struct Ray { float d; BLOCK_ID id; Ray(float _d, BLOCK_ID _id) { d = _d; id = _id; } };

Ray cast_ray(const Vec3<float>& pos, const Vec3<float>& dir) {
    // basically DDA for 3D

    float d = 0.0f;
    Vec3<int> rp((int)pos.x, (int)pos.y, (int)pos.z);
    Vec3<float> unit(fabsf(1 / dir.x), fabsf(1 / dir.y), fabsf(1 / dir.z));
    Vec3<float> step;
    Vec3<float> len;

    if (dir.x > 0) {
        step.x = 1.0f;
        len.x = (rp.x - pos.x + 1) * unit.x;
    }
    else {
        step.x = -1.0f;
        len.x = (pos.x - rp.x) * unit.x;
    }
    if (dir.y > 0) {
        step.y = 1.0f;
        len.y = (rp.y - pos.y + 1) * unit.y;
    }
    else {
        step.y = -1.0f;
        len.y = (pos.y - rp.y) * unit.y;
    }
    if (dir.z > 0) {
        step.z = 1.0f;
        len.z = (rp.z - pos.z + 1) * unit.z;
    }
    else {
        step.z = -1.0f;
        len.z = (pos.z - rp.z) * unit.z;
    }

    if (rp.x < 0 || rp.x > g_MAP_SIZE_X - 1 || rp.y < 0 || rp.y > g_MAP_SIZE_Y - 1 || rp.z < 0 || rp.z > g_MAP_SIZE_Z - 1)
        return Ray(0.f, 0);

    // quick NAN fix when multiplying by float inf
    if (dir.x == 0) len.x = INFINITY;
    if (dir.y == 0) len.y = INFINITY;
    if (dir.z == 0) len.z = INFINITY;

    BLOCK_ID blk;
    while (true) {
        blk = g_MAP.GetBlockAt((uint16_t)rp.x, (uint16_t)rp.y, (uint16_t)rp.z);

        if (blk != 0)
            return Ray(d, blk);

        if (len.x < len.y && len.x < len.z) {
            rp.x += (int)step.x;
            d = len.x;
            len.x += unit.x;
            if (rp.x < 0 || rp.x > g_MAP_SIZE_X - 1)
                return Ray(d, 0);
        }
        else if (len.y < len.z) {
            rp.y += (int)step.y;
            d = len.y;
            len.y += unit.y;
            if (rp.y < 0 || rp.y > g_MAP_SIZE_Y - 1)
                return Ray(d, 0);
        }
        else {
            rp.z += (int)step.z;
            d = len.z;
            len.z += unit.z;
            if (rp.z < 0 || rp.z > g_MAP_SIZE_Z - 1)
                return Ray(d, 0);
        }
    }
}

Vec3<float> get_normal(const Vec3<float>& pos) {
    // calculates the normal at the given XYZ position

    Vec3<float> normal;

    normal.x = (g_MAP.GetBlockAt((uint16_t)pos.z, (uint16_t)pos.y, (uint16_t)(pos.x - 1e-5f)) != 0) - (g_MAP.GetBlockAt((uint16_t)pos.z, (uint16_t)pos.y, (uint16_t)(pos.x + 1e-5f)) != 0);
    normal.y = (g_MAP.GetBlockAt((uint16_t)pos.z, (uint16_t)(pos.y - 1e-5f), (uint16_t)pos.x) != 0) - (g_MAP.GetBlockAt((uint16_t)pos.z, (uint16_t)(pos.y + 1e-5f), (uint16_t)pos.x) != 0);
    normal.z = (g_MAP.GetBlockAt((uint16_t)(pos.z - 1e-5f), (uint16_t)pos.y, (uint16_t)pos.x) != 0) - (g_MAP.GetBlockAt((uint16_t)(pos.z + 1e-5f), (uint16_t)pos.y, (uint16_t)pos.x) != 0);

    return normal;
}

Vec2<float> get_uv_tex_coord(const Vec3<float>& pos) {
    // calculates the uv texture coordinate at the given XYZ position

    Vec3<float> p(pos.x - (int)pos.x, pos.y - (int)pos.y, pos.z - (int)pos.z);
    Vec3<float> n = get_normal(pos);
    Vec2<float> coord(0.f);

    if (n.x != 0) {                 // left/right
        if (n.x > 0) {              // right
            coord.x = 1 - p.y;
            coord.y = p.z;
        }
        else {                      // left
            coord.x = p.y;
            coord.y = p.z;
        }
    }
    else if (n.y != 0) {            // forward/backward
        if (n.y > 0) {              // forward
            coord.x = p.x;
            coord.y = p.z;
        }
        else {                      // backward
            coord.x = 1 - p.x;
            coord.y = p.z;
        }
    }
    else if (n.z != 0) {            // up/down
        if (n.z > 0) {              // up
            coord.x = p.x;
            coord.y = p.y;
        }
        else {                      // down
            coord.x = 1 - p.x;
            coord.y = 1 - p.y;
        }
    }

    return coord;
}

Vec3<char> calculate_pixel(const Vec2<float>& coord) {
    // calculates the pixel at the given UV coordinate

    Vec3<float> dir(coord.x, g_CAMERA.GetFOV(), coord.y);
    dir = rotate_z(rotate_x(dir.norm(), g_CAMERA.m_Rotation.x), g_CAMERA.m_Rotation.z);       // rotates around 2 axis, the y axis will tilt the horizon (which is useless for now)

    Ray hit = cast_ray(g_CAMERA.m_Position, dir);
    Vec3<float> hit_point = dir * hit.d + g_CAMERA.m_Position;
    
    if (hit.id != 0) {
        Vec2<float> coord = get_uv_tex_coord(hit_point);

        return Vec3<char>((char)(coord.x * 255), (char)(coord.y * 255), (char)(0));
    }
    else {
        float h = hit_point.z / g_MAP_SIZE_Z;

        return Vec3<char>(0, (char)(h * 200.f), (char)(h * 255.f));
    }
}

void calculate_pixel_range(int begin, int end) {
    Vec2<float> coord;
    Vec3<char> color;
    for (register int y = begin; y < end; y++) {
        coord.y = -((float)y / g_WIN.GetWindowSize().y * 2 - 1);
        for (register int x = 0; x < g_WIN.GetWindowSize().x; x++) {
            coord.x = (float)x / g_WIN.GetWindowSize().x * 2 - 1;

            color = calculate_pixel(coord);

            g_WIN.GetScreenBuffer()[y * g_WIN.GetWindowSize().x * 4 + x * 4] = color.x;
            g_WIN.GetScreenBuffer()[y * g_WIN.GetWindowSize().x * 4 + x * 4 + 1] = color.y;
            g_WIN.GetScreenBuffer()[y * g_WIN.GetWindowSize().x * 4 + x * 4 + 2] = color.z;
            g_WIN.GetScreenBuffer()[y * g_WIN.GetWindowSize().x * 4 + x * 4 + 3] = 255;
        }
    }
}

int main() {
    // events
    sf::Event ev;

    // create buffer texture that we will update
    sf::Texture buffer;
    buffer.create(g_WIN.GetWindowSize().x, g_WIN.GetWindowSize().y);

    // create the buffer sprite
    sf::Sprite bufferSprite(buffer);

    // timers for calculating the framedelta
    std::chrono::steady_clock::time_point t1, t2;

    // generates a simple debug map
    generate_debug_chunk(g_MAP);

    // game loop
    while (g_WIN.GetWindow().isOpen()) {
        t1 = std::chrono::high_resolution_clock::now();

        // event polling
        while (g_WIN.GetWindow().pollEvent(ev)) {
            switch (ev.type) {
            case sf::Event::Closed:
                g_WIN.GetWindow().close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    g_WIN.GetWindow().close();
                break;
            }
        }

        // camera update
        g_CAMERA.OnUpdate(framedelta);
        
        // render frame
        std::thread worker1(calculate_pixel_range, 0, (int)(g_WIN.GetWindowSize().y * 1 / 6));
        std::thread worker2(calculate_pixel_range, (int)(g_WIN.GetWindowSize().y * 1 / 6), (int)(g_WIN.GetWindowSize().y * 2 / 6));
        std::thread worker3(calculate_pixel_range, (int)(g_WIN.GetWindowSize().y * 2 / 6), (int)(g_WIN.GetWindowSize().y * 3 / 6));
        std::thread worker4(calculate_pixel_range, (int)(g_WIN.GetWindowSize().y * 3 / 6), (int)(g_WIN.GetWindowSize().y * 4 / 6));
        std::thread worker5(calculate_pixel_range, (int)(g_WIN.GetWindowSize().y * 4 / 6), (int)(g_WIN.GetWindowSize().y * 5 / 6));
        std::thread worker6(calculate_pixel_range, (int)(g_WIN.GetWindowSize().y * 5 / 6), (int)(g_WIN.GetWindowSize().y));

        worker1.join();
        worker2.join();
        worker3.join();
        worker4.join();
        worker5.join();
        worker6.join();

        buffer.update(g_WIN.GetScreenBuffer()); // update the buffer

        g_WIN.GetWindow().draw(bufferSprite); // blit image sprite buffer to the screen

        g_WIN.GetWindow().display(); // tell app that window is done drawing

        // calculating the frame delta
        t2 = std::chrono::high_resolution_clock::now();
        framedelta = (t2 - t1).count() / 1e9f;
    }

    // end of application
    return 0;
}