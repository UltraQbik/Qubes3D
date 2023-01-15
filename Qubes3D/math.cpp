#include "math.h"


float q_isqrt(float n) {
	const float threehalfs = 1.5f;
	float y = n;

	long i = std::_Bit_cast<long>(y);

	i = 0x5f3759df - (i >> 1);
	y = std::_Bit_cast<float>(i);

	y = y * (threehalfs - ((n * 0.5f) * y * y));

	return y;
}

float clamp(float _x, float _min, float _max) {
	return max(_min, min(_max, _x));
}

float smoothstep(float _x, float _min, float _max, float t1, float t2) {
	float k = max(_min, min(_max, (_x - t1) / (t2 - t1)));
	return k * k * (3 - 2 * k);
}

float MathNoise(float _seed) {
	return std::fabsf(std::sinf(_seed * 5444343332.235f));
}
