#include <thread>
#include "Render.h"
#include "Camera.h"
#include "Window.h"
#include "World.h"
#include "AMath.h"


// temporary for tests
extern FVec3 g_Sun = FVec3(-2.f, 3.f, -4.f).normalize();


inline Ray castRay(const FVec3& pos, const FVec3& dir)
{
    float d = 0.0f;
    FVec3 rp(_mm_round_ps(pos.mmvalue, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
    FVec3 unit(_mm_div_ps(_mm_set_ps1(1.f), _mm_andnot_ps(_mm_set_ps1(-0.f), dir.mmvalue)));  // the _mm_rcp_ps is not accurate enough
    FVec3 step;
    FVec3 len;

    if (dir.x > 0) {
        step.x = 1;
        len.x = (rp.x - pos.x + 1) * unit.x;
    }
    else {
        step.x = -1;
        len.x = (pos.x - rp.x) * unit.x;
    }
    if (dir.y > 0) {
        step.y = 1;
        len.y = (rp.y - pos.y + 1) * unit.y;
    }
    else {
        step.y = -1;
        len.y = (pos.y - rp.y) * unit.y;
    }
    if (dir.z > 0) {
        step.z = 1;
        len.z = (rp.z - pos.z + 1) * unit.z;
    }
    else {
        step.z = -1;
        len.z = (pos.z - rp.z) * unit.z;
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
                return Ray(rp, (d) * dir + pos, blk, d);

            // check if we exceeded the max render distance
            if (d > g_CAM_RENDER_DISTANCE)
                return Ray(rp, d * dir + pos, 0, d);

            // move the ray
            if (len.x < len.y && len.x < len.z && dir.x != 0.f) {
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
            else if (len.y < len.z && dir.y != 0.f) {
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
    }
}

FVec3 getNormal(const FVec3& pos)
{
    // __m128i chunk_index = _mm_mullo_epi32(_mm_srli_epi32(_mm_cvttps_epi32(pos.mmvalue), g_CHUNK_RSH), _mm_set_epi32(0, g_MAP_SIZE * g_MAP_SIZE, g_MAP_SIZE, 1));
    // Chunk& chunk = g_World.getChunkArray()[_mm_extract_epi32(chunk_index, 0) + _mm_extract_epi32(chunk_index, 1) + _mm_extract_epi32(chunk_index, 2)];
    // FVec3 block_pos(std::fmodf(pos.x, g_CHUNK_SIZE), std::fmodf(pos.y, g_CHUNK_SIZE), std::fmodf(pos.z, g_CHUNK_SIZE));
    FVec3 norm(
        (float)((g_World.getBlockP((POS)(pos.x - 1.5e-5f), (POS)pos.y, (POS)pos.z) == 0) - (g_World.getBlockP((POS)(pos.x + 1.5e-5f), (POS)pos.y, (POS)pos.z) == 0)),
        (float)((g_World.getBlockP((POS)pos.x, (POS)(pos.y - 1.5e-5f), (POS)pos.z) == 0) - (g_World.getBlockP((POS)pos.x, (POS)(pos.y + 1.5e-5f), (POS)pos.z) == 0)),
        (float)((g_World.getBlockP((POS)pos.x, (POS)pos.y, (POS)(pos.z - 1.5e-5f)) == 0) - (g_World.getBlockP((POS)pos.x, (POS)pos.y, (POS)(pos.z + 1.5e-5f)) == 0))
    );

    return norm;
}


FVec3 calculatePixel(float u, float v)
{
	FVec3 dir = rotateZ(rotateX(FVec3(u, 1, v).normalize(), g_Camera.m_Dir.x), g_Camera.m_Dir.z);
    FVec3 final_color;

	Ray sample_ray = castRay(g_Camera.m_Pos, dir);

	if (sample_ray.cid == 0)
    {  // calculate the sky
        // calculate grayscale gradient for sky
        float grad = smoothstep(dir.z + 1.5f, 0.5f, 1.f, 0.f, 2.f);

        // calculate color gradient from (115, 135, 185) to (220, 240, 250)
        FVec3 sky = (FVec3(0.8627f, 0.9411f, 0.9803f) * grad) + (FVec3(0.4509f, 0.5294f, 0.7254f) * (1.f - grad));

        // calculate sun disc
        grad = clamp((dir - g_Sun).length() - 1.9f, 0.f, 1.f);

        // add sun to the sky
        sky += FVec3(grad);

        final_color = sky;
    }
    else
    {  // calculate the block
        final_color = getNormal(sample_ray.fpos) / 2.f + 0.5f;

        // basic post-processing (fog)
        float foggyness = std::expf(sample_ray.d - g_CAM_RENDER_DISTANCE) + 1.f;

        final_color *= foggyness;
    }

    return final_color;
}


void renderRange(int start, int end)
{
    sf::Uint8* buffer = g_Window.getScreenBuffer();
    uint16_t width = g_Window.getWidth(), height = g_Window.getHeight();
    float u, v, ratio = ((float)width / height);
    
    for (uint16_t y = start; y < end; y++)
    {
        v = 1.f - ((float)y / height * 2.f);
        for (uint16_t x = 0; x < width; x++)
        {
            u = ((float)x / width * 2.f - 1.f) * ratio;

            FVec3 color = clamp(calculatePixel(u, v)) * 255.f;

            buffer[y * width * 4 + x * 4] = (unsigned char)color.x;
            buffer[y * width * 4 + x * 4 + 1] = (unsigned char)color.y;
            buffer[y * width * 4 + x * 4 + 2] = (unsigned char)color.z;
            buffer[y * width * 4 + x * 4 + 3] = 255;
        }
    }
}


void renderFullCon()
{
    int thread_count = std::thread::hardware_concurrency() / 4;
    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    for (int i = 0; i < thread_count; i++)
        threads.emplace_back(std::thread(renderRange, (int)(g_Window.getHeight() * ((float)i / thread_count)), (int)(g_Window.getHeight() * ((float)(i + 1) / thread_count))));

    for (auto& th : threads)
        th.join();
}


void renderFull()
{
	sf::Uint8* buffer = g_Window.getScreenBuffer();
	uint16_t width = g_Window.getWidth(), height = g_Window.getHeight();
	float u, v, ratio = ((float)width / height);

	for (uint16_t y = 0; y < height; y++)
	{
		v = 1.f - ((float)y / height * 2.f);
		for (uint16_t x = 0; x < width; x++)
		{
			u = ((float)x / width * 2.f - 1.f) * ratio;

            FVec3 color = clamp(calculatePixel(u, v)) * 255.f;

			buffer[y * width * 4 + x * 4] = (unsigned char)color.x;
			buffer[y * width * 4 + x * 4 + 1] = (unsigned char)color.y;
			buffer[y * width * 4 + x * 4 + 2] = (unsigned char)color.z;
			buffer[y * width * 4 + x * 4 + 3] = 255;
		}
	}
}
