#pragma once
#include <SFML/Window.hpp>
#include "Config.h"
#include "Vector.h"


class Camera
{
public:
	Camera();
	Camera(FVec3 _pos);
	Camera(FVec3 _pos, FVec3 _dir);

	FVec3 m_Pos;
	FVec3 m_Dir;

	void onUpdate(float fd);
};


extern Camera g_Camera;
