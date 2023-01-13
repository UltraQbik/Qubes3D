#pragma once
#include <SFML/Graphics.hpp>

#include "vector.h"

class Camera {
public:
	Camera(float _fov);

	void OnUpdate(sf::RenderWindow& _win, float fd);

	float GetFOV() const { return m_FOV; }
	const Vec3<float>& GetPosition() const { return m_Position; }
	const Vec3<float>& GetRotation() const { return m_Rotation; }
private:
	// camera options
	float m_FOV;

	// camera spatial
	Vec3<float> m_Position;
	Vec3<float> m_Rotation;
};
