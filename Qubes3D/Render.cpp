#include "Render.h"
#include "Camera.h"
#include "Window.h"
#include "World.h"
#include "AMath.h"


Ray castRay(const FVec3& pos, const FVec3& dir)
{
    float d = 0.0f;
    FVec3 rp((POS)pos.x, (POS)pos.y, (POS)pos.z);
    FVec3 unit(fabsf(1 / dir.x), fabsf(1 / dir.y), fabsf(1 / dir.z));
    FVec3 step;
    FVec3 len;

    if (dir.x > 0) {
        step.x = 1;
        len.x = (rp.x - pos.x + 1) * unit.x;
    }
    else {
        step.x = -1;
        len.x = dir.x == 0 ? INFINITY : (pos.x - rp.x) * unit.x;
    }
    if (dir.y > 0) {
        step.y = 1;
        len.y = (rp.y - pos.y + 1) * unit.y;
    }
    else {
        step.y = -1;
        len.y = dir.y == 0 ? INFINITY : (pos.y - rp.y) * unit.y;
    }
    if (dir.z > 0) {
        step.z = 1;
        len.z = (rp.z - pos.z + 1) * unit.z;
    }
    else {
        step.z = -1;
        len.z = dir.z == 0 ? INFINITY : (pos.z - rp.z) * unit.z;
    }

    if (rp.x < 0 || rp.x >= g_CHUNK_SIZE * g_MAP_SIZE || rp.y < 0 || rp.y >= g_CHUNK_SIZE * g_MAP_SIZE || rp.z < 0 || rp.z >= g_CHUNK_SIZE * g_MAP_SIZE)
        return Ray(rp, pos, 0, 0.f);

    BID blk;
    while (true)
    {
        // fetch current chunk
        Chunk& chunk = g_World.getChunk((POS)rp.x, (POS)rp.y, (POS)rp.z);
        while (true) {
            // get the block from the current chunk
            blk = chunk.getBlock((POS)rp.x % g_CHUNK_SIZE, (POS)rp.y % g_CHUNK_SIZE, (POS)rp.z % g_CHUNK_SIZE);

            // check if the ray hit the block
            if (blk != 0)
                return Ray(rp, d * dir + pos, blk, d);

            // move the ray
            if (len.x < len.y && len.x < len.z) {
                rp.x += step.x;
                d = len.x;
                len.x += unit.x;

                // check if the ray is outside the world
                if (rp.x < 0 || rp.x >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(rp, d * dir + pos, blk, d);

                // check if the ray is outside the chunk
                if (rp.x >= g_CHUNK_SIZE)
                    break;
            }
            else if (len.y < len.z) {
                rp.y += step.y;
                d = len.y;
                len.y += unit.y;

                // check if the ray is outside the world
                if (rp.y < 0 || rp.y >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(rp, d * dir + pos, blk, d);

                // check if the ray is outside the chunk
                if (rp.y >= g_CHUNK_SIZE)
                    break;
            }
            else {
                rp.z += step.z;
                d = len.z;
                len.z += unit.z;

                // check if the ray is outside the world
                if (rp.z < 0 || rp.z >= g_CHUNK_SIZE * g_MAP_SIZE)
                    return Ray(rp, d * dir + pos, blk, d);

                // check if the ray is outside the chunk
                if (rp.z >= g_CHUNK_SIZE)
                    break;
            }
        }
        // check if we exceeded the max render distance
        if (d > g_CAM_RENDER_DISTANCE)
            return Ray(rp, d * dir + pos, blk, d);
    }
}


Vec3<float> calculatePixel(float u, float v)
{
	FVec3 dir(u, 1, v);
	dir.normalize();
	dir = rotateZ(rotateX(dir, g_Camera.m_Dir.x), g_Camera.m_Dir.z);

	Ray sample_ray = castRay(g_Camera.m_Pos, dir);

	if (sample_ray.cid == 0)
    {
        return Vec3<float>(0.f);
    }
    else
    {
        return Vec3<float>(1.f - smoothstep(sample_ray.d / 16.f));
    }
}


void render()
{
	sf::Uint8* buffer = g_Window.getScreenBuffer();
	uint16_t width = g_Window.getWidth(), height = g_Window.getHeight();
	float u, v;

	for (uint16_t y = 0; y < height; y++)
	{
		v = 1.f - ((float)y / height * 2.f);
		for (uint16_t x = 0; x < width; x++)
		{
			u = ((float)x / width * 2.f - 1.f) * (width / height);

			Vec3<float> color = calculatePixel(u, v) * 255.f;

			buffer[y * width * 4 + x * 4] = (unsigned char)color.x;
			buffer[y * width * 4 + x * 4 + 1] = (unsigned char)color.y;
			buffer[y * width * 4 + x * 4 + 2] = (unsigned char)color.z;
			buffer[y * width * 4 + x * 4 + 3] = 255;
		}
	}
}
