#pragma once
#include <SFML/Graphics.hpp>

#include "vector.h"


class Camera {
public:
	Camera(sf::RenderWindow& _win, float _fov);
	Camera(sf::RenderWindow& _win, float _fov, Vec3<float> _pos);
	Camera(sf::RenderWindow& _win, float _fov, Vec3<float> _pos, Vec3<float> _rot);

	void OnUpdate(float fd);

	float GetFOV() const { return m_FOV; }

	// camera spatial
	Vec3<float> m_Position;
	Vec3<float> m_Rotation;
private:
	// window
	sf::RenderWindow& m_Win;

	// camera options
	float m_FOV;
};
