#pragma once
#include "Vector.h"


/// Rotations
inline Vec3<float> rotateX(const Vec3<float>& point, float angle)
{  // Rotate around the X axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3<float>(point.x, point.y * cos - point.z * sin, point.z * cos + point.y * sin);
}

inline Vec3<float> rotateY(const Vec3<float>& point, float angle)
{  // Rotate around the Y axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3<float>(point.x * cos - point.z * sin, point.y, point.z * cos + point.x * sin);
}

inline Vec3<float> rotateZ(const Vec3<float>& point, float angle)
{  // Rotate around the Z axis
	float cos = std::cosf(angle), sin = std::sinf(angle);
	return Vec3<float>(point.x * cos - point.y * sin, point.y * cos + point.x * sin, point.z);
}


/// Shader math
template<typename T>
inline T clamp(T val, T _min = 0.f, T _max = 1.f)
{  // Clamps the values between _min and _max
	T v = val > _min ? val : _min;
	return v < _max ? v : _max;
}

inline Vec3<float> clamp(Vec3<float> val, float _min = 0.f, float _max = 1.f)
{  // Clamps the values between _min and _max
	return { clamp(val.x, _min, _max), clamp(val.y, _min, _max), clamp(val.z, _min, _max) };
}

inline float smoothstep(float val, float _min = 0.f, float _max = 1.f, float _t1 = 0.f, float _t2 = 1.f)
{  // Smoothstep function
	float k = clamp((val - _t1) / (_t2 - _t1), _min, _max);
	return k * k * (3 - 2 * k);
}