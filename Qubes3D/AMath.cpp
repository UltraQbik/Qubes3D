#include "amath.h"

/// Rotations
Vec3f rotateX(const Vec3f& point, float angle)
{  // Rotate around the X axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3f(point.x, point.y * cos - point.z * sin, point.z * cos + point.y * sin);
}

Vec3f rotateY(const Vec3f& point, float angle)
{  // Rotate around the Y axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3f(point.x * cos - point.z * sin, point.y, point.z * cos + point.x * sin);
}

Vec3f rotateZ(const Vec3f& point, float angle)
{  // Rotate around the Z axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3f(point.x * cos - point.y * sin, point.y * cos + point.x * sin, point.z);
}


/// Shader math
template<typename T>
T clamp(T val, T _min, T _max)
{  // Clamps the values between _min and _max
	T v = val > _min ? val : _min;
	return v < _max ? v : _max;
}

Vec3f clamp(Vec3f val, float _min, float _max)
{  // Clamps the values between _min and _max
	return { clamp(val.x, _min, _max), clamp(val.y, _min, _max), clamp(val.z, _min, _max) };
}

float smoothstep(float val, float _min, float _max, float _t1, float _t2)
{  // Smoothstep function
	float k = clamp((val - _t1) / (_t2 - _t1), _min, _max);
	return k * k * (3 - 2 * k);
}