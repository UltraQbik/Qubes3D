#include "Render.h"


Vec3<float> calculatePixel(float u, float v)
{
	Vec3<float> dir(u, 1, v);
	dir.normalize();
	dir = rotateZ(rotateX(dir, g_Camera.m_Dir.x), g_Camera.m_Dir.z);

	return dir;
}


void render()
{
	sf::Uint8* buffer = g_Window.getScreenBuffer();
	uint16_t width = g_Window.getWidth(), height = g_Window.getHeight();
	float u, v;

	for (uint16_t y = 0; y < height; y++)
	{
		v = (float)y / height * 2.f - 1.f;
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
