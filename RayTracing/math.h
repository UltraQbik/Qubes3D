#pragma once
#include "vector.h"


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))


float clamp(float _x, float _min = 0.f, float _max = 1.f);
float smoothstep(float _x, float t1 = 0.f, float t2 = 1.f, float _min = 0.f, float _max = 1.f);

float MathNoise(float _seed);
