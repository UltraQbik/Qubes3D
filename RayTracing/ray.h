#pragma once
#include "vector.h"

struct ray {
	Vec3 pos;
	Vec3 dir;
	float len;
	uint16_t cid;
};
