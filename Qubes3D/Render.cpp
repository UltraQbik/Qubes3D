#include "Render.h"
#include "Game.h"
#include "AMath.h"


Ray Game::castRay(const Vec3f& pos, const Vec3f& dir)
{
    Vec3<POS> ipos((POS)pos.x, (POS)pos.y, (POS)pos.z);

    // check if ray is outside the world
    if (ipos.x < 0 || ipos.x >= g_CHUNK_SIZE * g_MAP_SIZE || ipos.y < 0 || ipos.y >= g_CHUNK_SIZE * g_MAP_SIZE || ipos.z < 0 || ipos.z >= g_CHUNK_SIZE * g_MAP_SIZE)
        return Ray(ipos, pos, 0, 0.f);

    Vec3<POS> bpos;
    Vec3<POS> step;
    Vec3f unit(fabsf(1.f / dir.x), fabsf(1.f / dir.y), fabsf(1.f / dir.z));
    Vec3f len;
    float d = 0.f;

    if (dir.x > 0)
    {
        step.x = 1;
        len.x = (ipos.x - pos.x + 1) * unit.x;
    }
    else
    {
        step.x = -1;
        len.x = (pos.x - ipos.x) * unit.x;
    }
    if (dir.y > 0) {
        step.y = 1;
        len.y = (ipos.y - pos.y + 1) * unit.y;
    }
    else
    {
        step.y = -1;
        len.y = (pos.y - ipos.y) * unit.y;
    }
    if (dir.z > 0)
    {
        step.z = 1;
        len.z = (ipos.z - pos.z + 1) * unit.z;
    }
    else
    {
        step.z = -1;
        len.z = (pos.z - ipos.z) * unit.z;
    }

    BID blk = 0;
    while (true)
    {
        // fetch new chunk
        Chunk& fetched_chunk = m_World.getChunk(ipos);

        // calculate block position
        bpos.x = ipos.x % g_CHUNK_SIZE;
        bpos.y = ipos.y % g_CHUNK_SIZE;
        bpos.z = ipos.z % g_CHUNK_SIZE;
        while (true)
        {
            // fetch block
            blk = fetched_chunk.getBlock(bpos);

            // check if the ray hit the block
            if (blk != 0)
                return Ray(ipos, d * dir + pos, blk, d);

            // move the ray
            if (len.x < len.y && len.x < len.z && dir.x != 0.f) {
                ipos.x += step.x;
                bpos.x += step.x;
                d = len.x;
                len.x += unit.x;

                // check if the ray is outside the world
                if (ipos.x < 0 || ipos.x >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(ipos, d * dir + pos, 0, d);

                // check if the ray is outside the chunk
                if (bpos.x < 0 || bpos.x >= g_CHUNK_SIZE)
                    break;
            }
            else if (len.y < len.z && dir.y != 0.f) {
                ipos.y += step.y;
                bpos.y += step.y;
                d = len.y;
                len.y += unit.y;

                // check if the ray is outside the world
                if (ipos.y < 0 || ipos.y >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(ipos, d * dir + pos, 0, d);

                // check if the ray is outside the chunk
                if (bpos.y < 0 || bpos.y >= g_CHUNK_SIZE)
                    break;
            }
            else {
                ipos.z += step.z;
                bpos.z += step.z;
                d = len.z;
                len.z += unit.z;

                // check if the ray is outside the world
                if (ipos.z < 0 || ipos.z >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(ipos, d * dir + pos, 0, d);

                // check if the ray is outside the chunk
                if (bpos.z < 0 || bpos.z >= g_CHUNK_SIZE)
                    break;
            }

            // check if we exceeded the max render distance
            if (d > g_CAM_RENDER_DISTANCE)
                return Ray(ipos, d * dir + pos, 0, d);
        }
    }
}


Vec3f Game::getNormal(const Vec3f& pos)
{
    Vec3f norm(
        (float)((m_World.getBlockP((POS)(pos.x - 1.5e-5f), (POS)pos.y, (POS)pos.z) == 0) - (m_World.getBlockP((POS)(pos.x + 1.5e-5f), (POS)pos.y, (POS)pos.z) == 0)),
        (float)((m_World.getBlockP((POS)pos.x, (POS)(pos.y - 1.5e-5f), (POS)pos.z) == 0) - (m_World.getBlockP((POS)pos.x, (POS)(pos.y + 1.5e-5f), (POS)pos.z) == 0)),
        (float)((m_World.getBlockP((POS)pos.x, (POS)pos.y, (POS)(pos.z - 1.5e-5f)) == 0) - (m_World.getBlockP((POS)pos.x, (POS)pos.y, (POS)(pos.z + 1.5e-5f)) == 0))
    );

    return norm;
}


Vec3f Game::calculatePixel(float u, float v)
{
	Vec3f dir = rotateZ(rotateX(Vec3f(u, 1, v).norm(), m_Camera.m_Dir.x), m_Camera.m_Dir.z);
    Vec3f final_color;

	Ray sample_ray = castRay(m_Camera.m_Pos, dir);

	if (sample_ray.cid == 0)
    {  // calculate the sky
        // calculate grayscale gradient for sky
        float grad = smoothstep(dir.z + 1.5f, 0.5f, 1.f, 0.f, 2.f);

        // calculate color gradient from (115, 135, 185) to (220, 240, 250)
        Vec3f sky = (Vec3f(0.8627f, 0.9411f, 0.9803f) * grad) + (Vec3f(0.4509f, 0.5294f, 0.7254f) * (1.f - grad));

        // calculate sun disc
        grad = clamp((dir - m_Sun).length() - 1.9f, 0.f, 1.f);

        // add sun to the sky
        sky += Vec3f(grad);

        final_color = sky;
    }
    else
    {  // calculate the block
        // calculate dot product with sun
        float sun_dot = clamp(dotProduct(getNormal(sample_ray.fpos), m_Sun), 0.1f, 1.f);

        // cast shadow ray in direction, opposite to the sun
        Ray shadow_ray = castRay(sample_ray.fpos - dir * 1e-5f, -m_Sun);

        if (shadow_ray.cid == 0)
        {
            final_color = Vec3f(sun_dot);
        }
        else
        {
            final_color = Vec3f(0.1f);
        }
    }

    return final_color;
}


void Game::renderRange(int start, int end)
{
    float u, v, ratio = ((float)g_WINDOW_WIDTH / g_WINDOW_HEIGHT);
    
    for (uint16_t y = start; y < end; y++)
    {
        v = 1.f - ((float)y / g_WINDOW_HEIGHT * 2.f);
        for (uint16_t x = 0; x < g_WINDOW_WIDTH; x++)
        {
            u = ((float)x / g_WINDOW_WIDTH * 2.f - 1.f) * ratio;

            Vec3f color = clamp(calculatePixel(u, v)) * 255.f;

            m_ScreenBuffer[y * g_WINDOW_WIDTH * 4 + x * 4] = (unsigned char)color.x;
            m_ScreenBuffer[y * g_WINDOW_WIDTH * 4 + x * 4 + 1] = (unsigned char)color.y;
            m_ScreenBuffer[y * g_WINDOW_WIDTH * 4 + x * 4 + 2] = (unsigned char)color.z;
            m_ScreenBuffer[y * g_WINDOW_WIDTH * 4 + x * 4 + 3] = 255;
        }
    }
}
