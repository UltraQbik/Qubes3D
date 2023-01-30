#include "Render.h"


Ray castRay(const Vec3<float>& pos, const Vec3<float>& dir)
{
    float d = 0.0f;
    Vec3<POS> rp((POS)pos.x, (POS)pos.y, (POS)pos.z);
    Vec3<float> unit(fabsf(1 / dir.x), fabsf(1 / dir.y), fabsf(1 / dir.z));
    Vec3<POS> step;
    Vec3<float> len;

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

    if (rp.x < 0 || rp.x >= g_CHUNK_SIZE || rp.y < 0 || rp.y >= g_CHUNK_SIZE || rp.z < 0 || rp.z >= g_CHUNK_SIZE)
        return Ray(rp, pos, 0, 0.f);

    // quick NAN fix when multiplying by float inf
    if (dir.x == 0) len.x = INFINITY;
    if (dir.y == 0) len.y = INFINITY;
    if (dir.z == 0) len.z = INFINITY;

    BID blk;
    while (true) {
        blk = g_World.getBlock(rp);

        if (blk != 0)
            return Ray(rp, d * dir + pos, blk, d);

        if (len.x < len.y && len.x < len.z) {
            rp.x += (int)step.x;
            d = len.x;
            len.x += unit.x;
            if (rp.x < 0 || rp.x >= g_CHUNK_SIZE)
                return Ray(rp, d * dir + pos, blk, d);
        }
        else if (len.y < len.z) {
            rp.y += (int)step.y;
            d = len.y;
            len.y += unit.y;
            if (rp.y < 0 || rp.y >= g_CHUNK_SIZE)
                return Ray(rp, d * dir + pos, blk, d);
        }
        else {
            rp.z += (int)step.z;
            d = len.z;
            len.z += unit.z;
            if (rp.z < 0 || rp.z >= g_CHUNK_SIZE)
                return Ray(rp, d * dir + pos, blk, d);
        }
    }
}


Vec3<float> calculatePixel(float u, float v)
{
	Vec3<float> dir(u, 1, v);
	dir.normalize();
	dir = rotateZ(rotateX(dir, g_Camera.m_Dir.x), g_Camera.m_Dir.z);

	Ray sample_ray = castRay(g_Camera.m_Pos, dir);

    return Vec3<float>(smoothstep(4.f / sample_ray.d, 0.f, 0.9f));

	/*if (sample_ray.cid != 0)
		return Vec3<float>(8.f - smoothstep(sample_ray.d, 0.f, 8.f));
	else
		return Vec3<float>(0.88f);*/
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
