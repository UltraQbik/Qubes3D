#include "math.h"


float clamp(float _x, float _min, float _max) {
	return max(_min, min(_max, _x));
}

float smoothstep(float _x, float t1, float t2, float _min, float _max) {
	float k = max(_min, min(_max, (_x)));
	return k * k * (3 - 2 * k);
}

float MathNoise(float _seed) {
	return std::fabsf(std::sinf(_seed * 5444343332.235f));
}
