#pragma once
#include <SFML/Window.hpp>
#include "Config.h"
#include "Window.h"
#include "Vector.h"


class Camera
{
public:
	Camera();
	Camera(Vec3<float> _pos);
	Camera(Vec3<float> _pos, Vec3<float> _dir);

	Vec3<float> m_Pos;
	Vec3<float> m_Dir;

	void onUpdate(float fd);
};


extern Camera g_Camera;
