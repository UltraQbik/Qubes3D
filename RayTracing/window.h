#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

#include "vector.h"

class Window {
public:
	Window(Vec2<uint16_t> _win_size, unsigned char _fps_lim);

	void RecalculateWindowSize(uint16_t _res, uint16_t _h_ratio, uint16_t _v_ratio);
	void RecalculateWindowSize(Vec2<uint16_t> _win_size);
	void OnUpdate();

	float GetFrameDelta() { return m_FrameDelta; }

	sf::RenderWindow& GetWindow() { return m_Win; }
	sf::Uint8* GetScreenBuffer() { return m_ScreenBuffer; }
	const Vec2<uint16_t>& GetWindowSize() const { return m_WindowSize; }
private:
	char m_FpsLimit;

	float m_FrameDelta;
	std::chrono::steady_clock::time_point m_Timer = std::chrono::high_resolution_clock::now();

	sf::RenderWindow m_Win;

	sf::Texture m_TextureBuffer;		// is what being applied to m_SpriteBuffer
	sf::Sprite m_SpriteBuffer;			// is what being displayed on the window
	sf::Uint8* m_ScreenBuffer;			// is what stores the actual data

	Vec2<uint16_t> m_WindowSize;
};
