#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "window.h"

Window::Window(Vec2<uint16_t> _win_size, char _fps_lim)
	: m_Win(sf::VideoMode(_win_size.x, _win_size.y), "Qubes 3D [super]", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize) {
	m_WindowSize = _win_size;
	m_FpsLimit = _fps_lim;

	m_ScreenBuffer = new sf::Uint8[(uint64_t)m_WindowSize.x * m_WindowSize.y * 4];

	m_Win.setFramerateLimit(m_FpsLimit);
	m_Win.setMouseCursorGrabbed(true);
	m_Win.setMouseCursorVisible(false);
}

void Window::RecalculateWindowSize(uint16_t _res, uint16_t _h_ratio, uint16_t _v_ratio) {
	m_WindowSize.x = _h_ratio * _res;
	m_WindowSize.y = _v_ratio * _res;

	m_Win.setSize(sf::Vector2u(m_WindowSize.x, m_WindowSize.y));

	delete[] m_ScreenBuffer;
	m_ScreenBuffer = new sf::Uint8[(uint64_t)m_WindowSize.x * m_WindowSize.y * 4];
}

void Window::RecalculateWindowSize(Vec2<uint16_t> _win_size) {
	m_WindowSize = _win_size;

	m_Win.setSize(sf::Vector2u(m_WindowSize.x, m_WindowSize.y));

	delete[] m_ScreenBuffer;
	m_ScreenBuffer = new sf::Uint8[(uint64_t)m_WindowSize.x * m_WindowSize.y * 4];
}
