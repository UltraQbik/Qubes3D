#include "Window.h"


Window::Window(uint16_t _width, uint16_t _height, uint8_t _fps_cap)
	: m_Window(sf::VideoMode(_width, _height), "The Almighty Qubes 3D", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
{
	m_FpsLim = _fps_cap;

	m_Width = _width;
	m_Height = _height;

	m_FrameDelta = 0.00001f;
	m_Timer = std::chrono::high_resolution_clock::now();

	m_ScreenBuffer = new sf::Uint8[(uint64_t)m_Width * m_Height * 4];

	m_ScreenTexture.create(m_Width, m_Height);
	m_BufferSprite.setTexture(m_ScreenTexture);

	m_Window.setFramerateLimit(m_FpsLim);
	m_Window.setMouseCursorGrabbed(true);
	m_Window.setMouseCursorVisible(false);
}

Window::~Window()
{
	delete[] m_ScreenBuffer;
}


void Window::onUpdate()
{
	m_ScreenTexture.update(m_ScreenBuffer);			// update the texture
	m_Window.draw(m_BufferSprite);					// blit the sprite on screen
	m_Window.display();								// display everything

	m_FrameDelta = (std::chrono::high_resolution_clock::now() - m_Timer).count() * 1e-9f;
	m_Timer = std::chrono::high_resolution_clock::now();
}
