#pragma once
#include "Vector.h"


/// Rotations
inline Vec3<float> rotateX(const Vec3<float>& point, float angle)
{  // Rotate around the X axis
	return Vec3<float>(point.x, point.y * std::cosf(angle) - point.z * std::sinf(angle), point.z * std::cosf(angle) + point.y * std::sinf(angle));
}

inline Vec3<float> rotateY(const Vec3<float>& point, float angle)
{  // Rotate around the Y axis
	return Vec3<float>(point.x * std::cosf(angle) - point.z * std::sinf(angle), point.y, point.z * std::cosf(angle) + point.x * std::sinf(angle));
}

inline Vec3<float> rotateZ(const Vec3<float>& point, float angle)
{  // Rotate around the Z axis
	return Vec3<float>(point.x * std::cosf(angle) - point.y * std::sinf(angle), point.y * std::cosf(angle) + point.x * std::sinf(angle), point.z);
}

inline FVec3 rotateX(const FVec3& point, float _cos, float _sin)
{  // rotate around the X axis
	// calculate first product
	// X * 1.f, Y * cos, Z * cos
	// _mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, _cos, _cos, 1.f))

	// calculate second product
	// X * 0.f, Z * sin, Y * -sin
	// _mm_mul_ps(_mm_set_ps(0.f, point.y, point.z, 0.f), _mm_set_ps(0.f, -_sin, _sin, 0.f))

	// subtract first product from the second one
	// X - 0.f, Y * cos - Z * sin, Z * cos + Y * sin
	return _mm_sub_ps(_mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, _cos, _cos, 1.f)), _mm_mul_ps(_mm_set_ps(0.f, point.y, point.z, 0.f), _mm_set_ps(0.f, -_sin, _sin, 0.f)));
}

inline FVec3 rotateY(const FVec3& point, float _cos, float _sin)
{  // rotate around the Y axis
	// calculate first product
	// X * cos, Y * 1.f, Z * cos
	// _mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, _cos, 1.f, _cos))

	// calculate second product
	// Z * sin, Y * 0.f, X * -sin
	// _mm_mul_ps(_mm_set_ps(0.f, point.x, 0.f, point.z), _mm_set_ps(0.f, -_sin, 0.f, _sin))

	// subtract first product from the second one
	// X * cos - Z * sin, Y - 0.f, Z * cos + X * sin
	return _mm_sub_ps(_mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, _cos, 1.f, _cos)), _mm_mul_ps(_mm_set_ps(0.f, point.x, 0.f, point.z), _mm_set_ps(0.f, -_sin, 0.f, _sin)));
}

inline FVec3 rotateZ(const FVec3& point, float _cos, float _sin)
{  // rotate around the Y axis
	// calculate first product
	// X * cos, Y * cos, Z
	// _mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, 1.f, _cos, _cos))

	// calculate second product
	// Y * sin, X * -sin, 0.f
	// _mm_mul_ps(_mm_set_ps(0.f, 0.f, point.x, point.y), _mm_set_ps(0.f, 0.f, -_sin, _sin))

	// subtract first product from the second one
	// X * cos - Y * sin, Y * cos + X * sin, Z - 0.f
	return _mm_sub_ps(_mm_mul_ps(point.mmvalue, _mm_set_ps(0.f, 1.f, _cos, _cos)), _mm_mul_ps(_mm_set_ps(0.f, 0.f, point.x, point.y), _mm_set_ps(0.f, 0.f, -_sin, _sin)));
}

inline FVec3 rotateX(const FVec3& point, float angle)
{  // Rotate around the X axis
	return rotateX(point, std::cosf(angle), std::sinf(angle));
}

inline FVec3 rotateY(const FVec3& point, float angle)
{  // Rotate around the Y axis
	return rotateY(point, std::cosf(angle), std::sinf(angle));
}

inline FVec3 rotateZ(const FVec3& point, float angle)
{  // Rotate around the Z axis
	return rotateZ(point, std::cosf(angle), std::sinf(angle));
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

inline FVec3 clamp(FVec3 val, float _min = 0.f, float _max = 1.f)
{  // Clamps the values between _min and _max
	return _mm_min_ps(_mm_max_ps(val.mmvalue, _mm_set_ps1(_min)), _mm_set_ps1(_max));
}

inline float smoothstep(float val, float _min = 0.f, float _max = 1.f, float _t1 = 0.f, float _t2 = 1.f)
{  // Smoothstep function
	float k = clamp((val - _t1) / (_t2 - _t1), _min, _max);
	return k * k * (3 - 2 * k);
}