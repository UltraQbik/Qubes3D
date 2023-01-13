#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>

#include "configuration.h"
#include "camera.h"
#include "vector.h"


Camera::Camera(float _fov) {
	m_FOV = _fov;
}

void Camera::OnUpdate(sf::RenderWindow& _win, float fd) {
	Vec2<float> MouseMovement;
	
	MouseMovement.x = (float)sf::Mouse::getPosition(_win).x / _win.getSize().x - .5f;
	MouseMovement.y = (float)sf::Mouse::getPosition(_win).y / _win.getSize().y - .5f;

	sf::Mouse::setPosition(sf::Vector2i(_win.getSize().x / 2, _win.getSize().y / 2), _win);

	m_Rotation.x = std::fmodf(m_Rotation.x - MouseMovement.y * g_MOUSE_SENSITIVITY * fd, 6.2831853f);
	m_Rotation.z = std::fmodf(m_Rotation.z - MouseMovement.x * g_MOUSE_SENSITIVITY * fd, 6.2831853f);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_Position.x += std::cosf(m_Rotation.z + 1.5707963f) * g_CAMERA_SPEED * fd;
		m_Position.y += std::sinf(m_Rotation.z + 1.5707963f) * g_CAMERA_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_Position.x -= std::cosf(m_Rotation.z + 1.5707963f) * g_CAMERA_SPEED * fd;
		m_Position.y -= std::sinf(m_Rotation.z + 1.5707963f) * g_CAMERA_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_Position.x -= std::cosf(m_Rotation.z) * g_CAMERA_SPEED * fd;
		m_Position.y -= std::sinf(m_Rotation.z) * g_CAMERA_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_Position.x += std::cosf(m_Rotation.z) * g_CAMERA_SPEED * fd;
		m_Position.y += std::sinf(m_Rotation.z) * g_CAMERA_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		m_Position.z += g_CAMERA_SPEED * fd;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		m_Position.z -= g_CAMERA_SPEED * fd;
	}
}
