#pragma once
#include <SFML/Window.hpp>
#include "Objects.h"
#include "Config.h"
#include "Vector.h"


class Camera
{
public:
	Vec3<float> m_Pos;
	Vec3<float> m_Dir;

	void onUpdate(float fd);
};
