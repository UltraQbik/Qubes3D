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

inline Vec3<float> rotateXYZ(const Vec3<float>& point, const Vec3<float>& angle)
{  // Rotate around X, Y and Z axies
	return rotateZ(rotateY(rotateX(point, angle.x), angle.y), angle.z);
}

inline FVec3 rotateX(const FVec3& point, float angle)
{  // Rotate around the X axis
	return FVec3(point.x, point.y * std::cosf(angle) - point.z * std::sinf(angle), point.z * std::cosf(angle) + point.y * std::sinf(angle));
}

inline FVec3 rotateY(const FVec3& point, float angle)
{  // Rotate around the Y axis
	return FVec3(point.x * std::cosf(angle) - point.z * std::sinf(angle), point.y, point.z * std::cosf(angle) + point.x * std::sinf(angle));
}

inline FVec3 rotateZ(const FVec3& point, float angle)
{  // Rotate around the Z axis
	return FVec3(point.x * std::cosf(angle) - point.y * std::sinf(angle), point.y * std::cosf(angle) + point.x * std::sinf(angle), point.z);
}

inline FVec3 rotateXYZ(const FVec3& point, const FVec3& angle)
{  // Rotate around X, Y and Z axies
	return rotateZ(rotateY(rotateX(point, angle.x), angle.y), angle.z);
}


/// Shader math
template<typename T>
inline T clamp(T val, T _min = 0.f, T _max = 1.f)
{  // Clamps the values between _min and _max
	T v = val > _min ? val : _min;
	return v < _max ? v : _max;
}

inline float smoothstep(float val, float _min = 0.f, float _max = 1.f, float _t1 = 0.f, float _t2 = 1.f)
{  // Smoothstep function
	float k = clamp((val - _t1) / (_t2 - _t1), _min, _max);
	return k * k * (3 - 2 * k);
}