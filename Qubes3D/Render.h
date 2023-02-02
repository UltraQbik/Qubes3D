#pragma once
#include "Config.h"
#include "Vector.h"



struct Ray
{
	Ray(FVec3 _ipos, FVec3 _fpos, BID _cid, float _d) { ipos = _ipos; fpos = _fpos; cid = _cid; d = _d; }

	FVec3 ipos;			// integer position
	FVec3 fpos;			// float position
	BID cid;			// collision id
	float d;			// distance
};

inline Ray castRay(const FVec3& pos, const FVec3& dir);

void render();
