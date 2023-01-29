#include "Camera.h"


Camera::Camera()
{
	m_Pos = Vec3<float>(0);
	m_Dir = Vec3<float>(0, 1, 0);
}

Camera::Camera(Vec3<float> _pos)
{
	m_Pos = _pos;
	m_Dir = Vec3<float>(0, 1, 0);
}

Camera::Camera(Vec3<float> _pos, Vec3<float> _dir)
{
	m_Pos = _pos;
	m_Dir = _dir.norm();
}


void Camera::onUpdate(float fd)
{
	// mouse controls
	float mouse_x, mouse_y;

	mouse_x = (float)sf::Mouse::getPosition(g_Window.getWindow()).x / g_Window.getWidth() - 0.5f;
	mouse_y = (float)sf::Mouse::getPosition(g_Window.getWindow()).y / g_Window.getHeight() - 0.5f;

	sf::Mouse::setPosition(sf::Vector2i(g_Window.getWidth() / 2, g_Window.getHeight() / 2), g_Window.getWindow());

	m_Dir.x = std::fmodf(m_Dir.x + mouse_y * g_CAM_SENSITIVITY * fd, 6.2831853f);
	m_Dir.z = std::fmodf(m_Dir.z - mouse_x * g_CAM_SENSITIVITY * fd, 6.2831853f);

	// keyboard controls
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_Pos.x += std::sinf(m_Dir.z) * g_CAM_SPEED * fd;
		m_Pos.y += std::cosf(m_Dir.z) * g_CAM_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_Pos.x -= std::sinf(m_Dir.z) * g_CAM_SPEED * fd;
		m_Pos.y -= std::cosf(m_Dir.z) * g_CAM_SPEED * fd;
	}
}
