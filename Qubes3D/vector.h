#pragma once
#include <cmath>


// Vector XYZ (vector 3)
template<typename T>
struct Vec3 {
	T x, y, z;

	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(T _x) { x = _x; y = _x; z = _x; }
	Vec3(T _x, T _y) { x = _x; y = _y; z = 0; }
	Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

	float length() { return sqrtf(x * x + y * y + z * z); }
	Vec3<T> norm() { float iL = 1 / sqrtf(x * x + y * y + z * z); return { (T)(x * iL), (T)(y * iL), (T)(z * iL) }; }
	void normalize() { float iL = 1 / sqrtf(x * x + y * y + z * z); x *= iL; y *= iL; z *= iL; }
};

template<typename T1>
Vec3<T1> operator-(const Vec3<T1>& lhs) { return { -lhs.x, -lhs.y, -lhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator/(const Vec3<T1>& lhs, const Vec3<T2> rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator+=(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator-=(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator*=(const Vec3<T1>& lhs, const Vec3<T2>& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator/=(const Vec3<T1>& lhs, const Vec3<T2> rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator+(const Vec3<T1>& lhs, T2 rhs) { return { lhs.x + rhs, lhs.y + rhs, lhs.z + rhs }; }

template<typename T1, typename T2>
Vec3<T1> operator-(const Vec3<T1>& lhs, T2 rhs) { return { lhs.x - rhs, lhs.y - rhs, lhs.z - rhs }; }

template<typename T1, typename T2>
Vec3<T1> operator*(const Vec3<T1>& lhs, T2 rhs) { return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs }; }

template<typename T1, typename T2>
Vec3<T1> operator/(const Vec3<T1>& lhs, T2 rhs) { return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs }; }

template<typename T1, typename T2>
Vec3<T1> operator+(T2 lhs, const Vec3<T1>& rhs) { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator-(T2 lhs, const Vec3<T1>& rhs) { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator*(T2 lhs, const Vec3<T1>& rhs) { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }

template<typename T1, typename T2>
Vec3<T1> operator/(T2 lhs, const Vec3<T1>& rhs) { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }


template<typename T1, typename T2>
float dotProduct(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}


// Vector XY (vector 2)
template<typename T>
struct Vec2 {
	T x, y;

	Vec2() { x = 0; y = 0; }
	Vec2(T _x) { x = _x; y = _x; }
	Vec2(T _x, T _y) { x = _x; y = _y; }

	float length() { return sqrtf(x * x + y * y); }
	Vec3<T> norm() { float iL = 1 / sqrtf(x * x + y * y); return { (T)(x * iL), (T)(y * iL) }; }
	void normalize() { float iL = 1 / sqrtf(x * x + y * y); x *= iL; y *= iL; }
};

template<typename T1>
Vec2<T1> operator-(const Vec2<T1>& lhs) { return { -lhs.x, -lhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator+(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator-(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator*(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator/(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator+=(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator-=(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator*=(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator/=(const Vec2<T1>& lhs, const Vec2<T2>& rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator+(const Vec2<T1>& lhs, T2 rhs) { return { lhs.x + rhs, lhs.y + rhs }; }

template<typename T1, typename T2>
Vec2<T1> operator-(const Vec2<T1>& lhs, T2 rhs) { return { lhs.x - rhs, lhs.y - rhs }; }

template<typename T1, typename T2>
Vec2<T1> operator*(const Vec2<T1>& lhs, T2 rhs) { return { lhs.x * rhs, lhs.y * rhs }; }

template<typename T1, typename T2>
Vec2<T1> operator/(const Vec2<T1>& lhs, T2 rhs) { return { lhs.x / rhs, lhs.y / rhs }; }

template<typename T1, typename T2>
Vec2<T1> operator+(T2 lhs, const Vec2<T1>& rhs) { return { lhs + rhs.x, lhs + rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator-(T2 lhs, const Vec2<T1>& rhs) { return { lhs - rhs.x, lhs - rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator*(T2 lhs, const Vec2<T1>& rhs) { return { lhs * rhs.x, lhs * rhs.y }; }

template<typename T1, typename T2>
Vec2<T1> operator/(T2 lhs, const Vec2<T1>& rhs) { return { lhs / rhs.x, lhs / rhs.y }; }

template<typename T1, typename T2>
float dotProduct(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}


// probably will be moved in the future
inline Vec3<float> rotateX(const Vec3<float>& point, float angle) {
	return Vec3<float>(point.x, point.y * std::cosf(angle) - point.z * std::sinf(angle), point.z * std::cosf(angle) + point.y * std::sinf(angle));
}

inline Vec3<float> rotateY(const Vec3<float>& point, float angle) {
	return Vec3<float>(point.x * std::cosf(angle) - point.z * std::sinf(angle), point.y, point.z * std::cosf(angle) + point.x * std::sinf(angle));
}

inline Vec3<float> rotateZ(const Vec3<float>& point, float angle) {
	return Vec3<float>(point.x * std::cosf(angle) - point.y * std::sinf(angle), point.y * std::cosf(angle) + point.x * std::sinf(angle), point.z);
}

inline Vec3<float> rotateXYZ(const Vec3<float>& point, const Vec3<float>& angle) {
	return rotateZ(rotateY(rotateX(point, angle.x), angle.y), angle.z);
}