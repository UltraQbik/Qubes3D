#include "Render.h"


void debugScreenTest()
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

			buffer[y * width * 4 + x * 4] = u * 127.5f + 127.5f;
			buffer[y * width * 4 + x * 4 + 1] = v * 127.5f + 127.5f;
			buffer[y * width * 4 + x * 4 + 2] = 0;
			buffer[y * width * 4 + x * 4 + 3] = 255;
		}
	}
}
