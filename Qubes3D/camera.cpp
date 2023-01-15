#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "configuration.h"
#include "camera.h"
#include "vector.h"


Camera::Camera(sf::RenderWindow& _win, float _fov)
	: m_Win(_win) {
	m_FOV = _fov;
}

Camera::Camera(sf::RenderWindow& _win, float _fov, Vec3<float> _pos)
	: m_Win(_win) {
	m_FOV = _fov;

	m_Position = _pos;
}

Camera::Camera(sf::RenderWindow& _win, float _fov, Vec3<float> _pos, Vec3<float> _rot)
	: m_Win(_win) {
	m_FOV = _fov;

	m_Position = _pos;
	m_Rotation = _rot;
}

void Camera::OnUpdate(float fd) {
	if (!m_Win.hasFocus())
		return;

	Vec2<float> mouseMovement;
	
	mouseMovement.x = (float)sf::Mouse::getPosition(m_Win).x / m_Win.getSize().x - .5f;
	mouseMovement.y = (float)sf::Mouse::getPosition(m_Win).y / m_Win.getSize().y - .5f;

	sf::Mouse::setPosition(sf::Vector2i(m_Win.getSize().x / 2, m_Win.getSize().y / 2), m_Win);

	m_Rotation.x = std::fmodf(m_Rotation.x - mouseMovement.y * g_MOUSE_SENSITIVITY * fd, 6.2831853f);
	m_Rotation.z = std::fmodf(m_Rotation.z - mouseMovement.x * g_MOUSE_SENSITIVITY * fd, 6.2831853f);


	float movementSpeed = g_CAMERA_SPEED * (g_CAMERA_SHIFT_MUL * sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) + 1.f) * fd;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_Position.x += std::cosf(m_Rotation.z + 1.5707963f) * movementSpeed;
		m_Position.y += std::sinf(m_Rotation.z + 1.5707963f) * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_Position.x -= std::cosf(m_Rotation.z + 1.5707963f) * movementSpeed;
		m_Position.y -= std::sinf(m_Rotation.z + 1.5707963f) * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_Position.x -= std::cosf(m_Rotation.z) * movementSpeed;
		m_Position.y -= std::sinf(m_Rotation.z) * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_Position.x += std::cosf(m_Rotation.z) * movementSpeed;
		m_Position.y += std::sinf(m_Rotation.z) * movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		m_Position.z += movementSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		m_Position.z -= movementSpeed;
	}
}
