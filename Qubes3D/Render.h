#pragma once
#include "config.h"
#include "vector.h"


struct Ray
{
	Ray(Vec3<POS> _ipos, Vec3f _fpos, BID _cid, float _d) { ipos = _ipos; fpos = _fpos; cid = _cid; d = _d; }

	Vec3<POS> ipos;				// integer position
	Vec3f fpos;					// float position
	BID cid;					// collision id
	float d;					// distance
};