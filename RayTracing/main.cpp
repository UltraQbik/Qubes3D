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
#include "math.h"


// window
Window g_WIN(Vec2<uint16_t>(1280, 720), 75);

// player camera
Camera g_CAMERA(g_WIN.GetWindow(), 0.8f, Vec3<float>(g_CHUNK_SIZE_X / 2.f, g_CHUNK_SIZE_Y / 2.f, g_CHUNK_SIZE_Z / 2.f + 1));

// world (just a chunk for now)
Chunk g_MAP(0);

// sun vector (temp)
static Vec3<float> g_SUN = Vec3<float>(2.f, 3.f, -4.f).norm();


// ray hit data
struct Ray {
    float d;
    BLOCK_ID id;
    Vec3<uint16_t> ipos;
    Vec3<float> fpos;
    
    Ray(float _d, BLOCK_ID _id, Vec3<uint16_t> _ipos, Vec3<float> _fpos) {
        d = _d;         // distance
        id = _id;       // block id (0 if sky)
        ipos = _ipos;   // integer position
        fpos = _fpos;   // float position (surfrace)
    }
};

Ray rayCast(const Vec3<float>& pos, const Vec3<float>& dir) {
    // basically DDA for 3D

    float d = 0.0f;
    Vec3<uint16_t> rp((uint16_t)pos.x, (uint16_t)pos.y, (uint16_t)pos.z);
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

    if (rp.x <= 0 || rp.x >= g_CHUNK_SIZE_X || rp.y <= 0 || rp.y >= g_CHUNK_SIZE_Y || rp.z <= 0 || rp.z >= g_CHUNK_SIZE_Z)
        return Ray(0.f, 0, rp, pos);

    // quick NAN fix when multiplying by float inf
    if (dir.x == 0) len.x = INFINITY;
    if (dir.y == 0) len.y = INFINITY;
    if (dir.z == 0) len.z = INFINITY;

    BLOCK_ID blk;
    while (true) {
        blk = g_MAP.GetBlockAt(rp.x, rp.y, rp.z);

        if (blk != 0)
            return Ray(d, blk, rp, (d - 1e-5f) * dir + pos);

        if (len.x < len.y && len.x < len.z) {
            rp.x += (int)step.x;
            d = len.x;
            len.x += unit.x;
            if (rp.x <= 0 || rp.x >= g_CHUNK_SIZE_X)
                return Ray(d, 0, rp, d * dir + pos);
        }
        else if (len.y < len.z) {
            rp.y += (int)step.y;
            d = len.y;
            len.y += unit.y;
            if (rp.y <= 0 || rp.y >= g_CHUNK_SIZE_Y)
                return Ray(d, 0, rp, d * dir + pos);
        }
        else {
            rp.z += (int)step.z;
            d = len.z;
            len.z += unit.z;
            if (rp.z <= 0 || rp.z >= g_CHUNK_SIZE_Z)
                return Ray(d, 0, rp, d * dir + pos);
        }
    }
}

Vec3<float> getNormal(const Vec3<float>& pos) {
    Vec3<float> normal;

    normal.x = (g_MAP.GetBlockAt((uint16_t)(pos.x - 2.5e-5f), (uint16_t)(pos.y), (uint16_t)(pos.z)) == 0) - (g_MAP.GetBlockAt((uint16_t)(pos.x + 2.5e-5f), (uint16_t)(pos.y), (uint16_t)(pos.z)) == 0);
    normal.y = (g_MAP.GetBlockAt((uint16_t)(pos.x), (uint16_t)(pos.y - 2.5e-5f), (uint16_t)(pos.z)) == 0) - (g_MAP.GetBlockAt((uint16_t)(pos.x), (uint16_t)(pos.y + 2.5e-5f), (uint16_t)(pos.z)) == 0);
    normal.z = (g_MAP.GetBlockAt((uint16_t)(pos.x), (uint16_t)(pos.y), (uint16_t)(pos.z - 2.5e-5f)) == 0) - (g_MAP.GetBlockAt((uint16_t)(pos.x), (uint16_t)(pos.y), (uint16_t)(pos.z + 2.5e-5f)) == 0);

    return normal;
}

Vec2<float> getUVTextureCoord(const Vec3<float>& pos, const Vec3<float>& normal) {
    Vec3<float> p(pos.x - (int)pos.x, pos.y - (int)pos.y, pos.z - (int)pos.z);
    Vec2<float> coord(0.f);

    if (normal.x != 0) {        // left/right
        if (normal.x > 0) {     // right
            coord.x = 1 - p.y;
            coord.y = p.z;
        }
        else {                  // left
            coord.x = p.y;
            coord.y = p.z;
        }
    }
    else if (normal.y != 0) {   // forward/backward
        if (normal.y > 0) {     // forward
            coord.x = p.x;
            coord.y = p.z;
        }
        else {                  // backward
            coord.x = 1 - p.x;
            coord.y = p.z;
        }
    }
    else if (normal.z != 0) {   // up/down
        if (normal.z > 0) {     // up
            coord.x = p.x;
            coord.y = p.y;
        }
        else {                  // down
            coord.x = 1 - p.x;
            coord.y = 1 - p.y;
        }
    }

    return coord;
}

Vec2<float> getUVTextureCoord(const Vec3<float>& pos) {
    return getUVTextureCoord(pos, getNormal(pos));
}

Vec3<COLOR> calculatePixelAt(const Vec2<float>& coord) {
    // add a tiny cursor
    if (fabsf(coord.x) < 0.01 && fabsf(coord.y) < 0.01)
        return Vec3<COLOR>(255, 0, 255);

    // calculate ray direction
    Vec3<float> dir(coord.x, g_CAMERA.GetFOV(), coord.y);
    dir = rotateZ(rotateX(dir.norm(), g_CAMERA.m_Rotation.x), g_CAMERA.m_Rotation.z);

    // cast main ray
    Ray main_ray = rayCast(g_CAMERA.m_Position, dir);

    if (main_ray.id != 0) {  // hit block
        // cast shadow ray (the ray that calculates the shadow)
        Vec3<float> shadow_hit_pos = main_ray.fpos - dir * 5e-5f;
        Ray shadow_ray = rayCast(shadow_hit_pos, -g_SUN);

        // TODO: add textures
        Vec3<float> surf_norm = getNormal(main_ray.fpos);
        // Vec2<float> coord = getUVTextureCoord(main_ray.fpos, surf_norm);

        // calculates how similar the sun and the normal vectors are
        float dot = smoothstep(vecDot(surf_norm, g_SUN), g_AMBIENT);

        float brightness = 0.f;
        if (shadow_ray.id == 0) {  // shadow ray didn't hit a block
            brightness = dot;
        }
        else {  // shadow ray hit a block
            brightness = smoothstep(1 / q_isqrt(shadow_ray.d), g_AMBIENT, 1.f, 0.f, 2.f) * dot;
        }

        return Vec3<COLOR>((COLOR)(clamp(brightness, g_AMBIENT) * 255.f));
    }
    else {  // didn't hit a block
        float h = smoothstep((dir.z * 16.f + 16.f) / g_MAP_SIZE_Z, .2f);

        return Vec3<COLOR>(0, (COLOR)(h * 200.f), (COLOR)(h * 255.f));
    }
}

void calculatePixelRange(int begin, int end) {
    Vec2<float> coord;
    Vec3<COLOR> color;
    for (register int y = begin; y < end; y++) {
        coord.y = -((float)y / g_WIN.GetWindowSize().y * 2 - 1);
        for (register int x = 0; x < g_WIN.GetWindowSize().x; x++) {
            coord.x = ((float)x / g_WIN.GetWindowSize().x * 2 - 1) * ((float)g_WIN.GetWindowSize().x / g_WIN.GetWindowSize().y);

            color = calculatePixelAt(coord);

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

    // generates a simple debug map
    generateFlatChunk(g_MAP);

    // game loop
    while (g_WIN.GetWindow().isOpen()) {
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
            case sf::Event::MouseButtonPressed:
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    Vec3<float> dir = rotateZ(rotateX(Vec3<float>(0.f, 1.f, 0.f), g_CAMERA.m_Rotation.x), g_CAMERA.m_Rotation.z);
                    auto hit = rayCast(g_CAMERA.m_Position, dir);
                    auto normal = getNormal(hit.fpos);

                    g_MAP.SetBlockAt(hit.ipos.x - normal.x, hit.ipos.y - normal.y, hit.ipos.z - normal.z, 1);
                }
                if (ev.mouseButton.button == sf::Mouse::Right) {
                    Vec3<float> dir = rotateZ(rotateX(Vec3<float>(0.f, 1.f, 0.f), g_CAMERA.m_Rotation.x), g_CAMERA.m_Rotation.z);
                    auto hit = rayCast(g_CAMERA.m_Position, dir);
                    auto normal = getNormal(hit.fpos);

                    g_MAP.SetBlockAt(hit.ipos.x, hit.ipos.y, hit.ipos.z, 0);
                }
                if (ev.mouseButton.button == sf::Mouse::Middle) {
                    Vec3<float> dir = rotateZ(rotateX(Vec3<float>(0.f, 1.f, 0.f), g_CAMERA.m_Rotation.x), g_CAMERA.m_Rotation.z);
                    auto hit = rayCast(g_CAMERA.m_Position, dir);
                    auto normal = getNormal(hit.fpos);

                    printf("pos: %f, %f, %f; norm: %f, %f, %f\n", hit.fpos.x, hit.fpos.y, hit.fpos.z, normal.x, normal.y, normal.z);
                    printf("dot: %f\n", smoothstep(vecDot(getNormal(hit.fpos), g_SUN)));
                }
            }
        }

        // camera update
        g_CAMERA.OnUpdate(g_WIN.GetFrameDelta());
        
        // render frame
        std::thread worker1(calculatePixelRange, 0, (int)(g_WIN.GetWindowSize().y * 1 / 8));
        std::thread worker2(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 1 / 8), (int)(g_WIN.GetWindowSize().y * 2 / 8));
        std::thread worker3(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 2 / 8), (int)(g_WIN.GetWindowSize().y * 3 / 8));
        std::thread worker4(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 3 / 8), (int)(g_WIN.GetWindowSize().y * 4 / 8));
        std::thread worker5(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 4 / 8), (int)(g_WIN.GetWindowSize().y * 5 / 8));
        std::thread worker6(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 5 / 8), (int)(g_WIN.GetWindowSize().y * 6 / 8));
        std::thread worker7(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 6 / 8), (int)(g_WIN.GetWindowSize().y * 7 / 8));
        std::thread worker8(calculatePixelRange, (int)(g_WIN.GetWindowSize().y * 7 / 8), (int)(g_WIN.GetWindowSize().y));

        worker1.join();
        worker2.join();
        worker3.join();
        worker4.join();
        worker5.join();
        worker6.join();
        worker7.join();
        worker8.join();

        // update the window
        g_WIN.OnUpdate();
    }

    // end of application
    return 0;
}