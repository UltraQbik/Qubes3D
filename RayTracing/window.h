#pragma once
#include <SFML/Graphics.hpp>

#include "vector.h"

class Window {
public:
	Window(Vec2<uint16_t> _win_size, char _fps_lim);

	void RecalculateWindowSize(uint16_t _res, uint16_t _h_ratio, uint16_t _v_ratio);
	void RecalculateWindowSize(Vec2<uint16_t> _win_size);

	sf::RenderWindow& GetWindow() { return m_Win; }
	sf::Uint8* GetScreenBuffer() { return m_ScreenBuffer; }
	const Vec2<uint16_t>& GetWindowSize() const { return m_WindowSize; }
private:
	char m_FpsLimit;

	sf::RenderWindow m_Win;
	Vec2<uint16_t> m_WindowSize;
	sf::Uint8* m_ScreenBuffer;
};
