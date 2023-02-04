#include "Camera.h"
#include "Window.h"
#include "World.h"
#include "AMath.h"
#include "Render.h"


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

	m_Dir.x = std::fmodf(m_Dir.x - mouse_y * g_CAM_SENSITIVITY * fd, 6.2831853f);
	m_Dir.x = clamp(m_Dir.x, -1.5f, 1.5f);
	m_Dir.z = std::fmodf(m_Dir.z - mouse_x * g_CAM_SENSITIVITY * fd, 6.2831853f);

	// keyboard movement
	float movement_speed = g_CAM_SPEED * fd;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_Pos.x += std::cosf(m_Dir.z + 1.5707963f) * movement_speed;
		m_Pos.y += std::sinf(m_Dir.z + 1.5707963f) * movement_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_Pos.x -= std::cosf(m_Dir.z + 1.5707963f) * movement_speed;
		m_Pos.y -= std::sinf(m_Dir.z + 1.5707963f) * movement_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_Pos.x -= std::cosf(m_Dir.z) * movement_speed;
		m_Pos.y -= std::sinf(m_Dir.z) * movement_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_Pos.x += std::cosf(m_Dir.z) * movement_speed;
		m_Pos.y += std::sinf(m_Dir.z) * movement_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		m_Pos.z += movement_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		m_Pos.z -= movement_speed;
	}

	// Check for events (god this looks awful)
	sf::Event ev;
	while (g_Window.getWindow().pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			g_Window.getWindow().close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				g_Window.getWindow().close();
			break;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				Ray ray = castRay(m_Pos, rotateZ(rotateX(Vec3<float>(0, 1, 0), m_Dir.x), m_Dir.z));
				Vec3<float> normal = getNormal(ray.fpos);
				Vec3<POS> block_pos = ray.ipos - Vec3<POS>((POS)normal.x, (POS)normal.y, (POS)normal.z);
				g_World.setBlockP(block_pos, 1);
			}
			if (ev.mouseButton.button == sf::Mouse::Right)
			{
				Ray ray = castRay(m_Pos, rotateZ(rotateX(Vec3<float>(0, 1, 0), m_Dir.x), m_Dir.z));
				g_World.setBlockP(ray.ipos, 0);
			}
		}
	}
}
