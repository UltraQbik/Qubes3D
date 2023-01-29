#pragma once
#include "Camera.h"
#include "Window.h"
#include "World.h"
#include "Config.h"
#include "Vector.h"



struct Ray
{
	Ray(Vec3<POS> _ipos, Vec3<float> _fpos, BID _cid, float _d) { ipos = _ipos; fpos = _fpos; cid = _cid; d = _d; }

	Vec3<POS> ipos;			// integer position
	Vec3<float> fpos;		// float position
	BID cid;				// collision id
	float d;				// distance
};

void render();