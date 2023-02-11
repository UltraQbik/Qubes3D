#pragma once
#include "vector.h"


class Camera
{
public:
	Camera() { m_Pos = Vec3f(0.f), m_Dir = Vec3f(0.f); };

	Vec3f m_Pos;
	Vec3f m_Dir;
};
