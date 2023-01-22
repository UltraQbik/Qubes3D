#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Vector.h"


class Window
{
public:
	Window(uint16_t _width, uint16_t _height, uint8_t _fps_cap);

	void onUpdate();

	float getDelta() { return m_FrameDelta; }
	uint16_t getWidth() { return m_Width; }
	uint16_t getHeight() { return m_Height; }

	sf::RenderWindow& getWindow() { return m_Window; }
	sf::Uint8* getScreenBuffer() { return m_ScreenBuffer; }
private:
	uint8_t m_FpsLim;
	uint16_t m_Width, m_Height;

	sf::RenderWindow m_Window;

	float m_FrameDelta;
	std::chrono::steady_clock::time_point m_Timer;

	sf::Texture m_ScreenTexture;
	sf::Sprite m_BufferSprite;
	sf::Uint8* m_ScreenBuffer;
};

