#pragma once
#include <type_traits>
#include <cmath>

#include "math.h"



template<typename T>
struct Vec3 {
	T x, y, z;

	Vec3(T a, T b, T c) { x = a; y = b; z = c; }
	Vec3(T a) { x = a; y = a; z = a; }
	Vec3() { x = 0; y = 0; z = 0; }
	T len() { return sqrt(x * x + y * y + z * z); }
	Vec3<T> norm() { float l = q_isqrt(x * x + y * y + z * z); return { x * l, y * l, z * l }; }
};

template<typename T1, typename T2>
auto operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs) -> Vec3<decltype(std::declval<T1>() + std::declval<T2>())>
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template<typename T1, typename T2>
auto operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs) -> Vec3<decltype(std::declval<T1>() - std::declval<T2>())>
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template<typename T1>
auto operator-(const Vec3<T1>& lhs) -> Vec3<decltype(-std::declval<T1>())>
{
	return { -lhs.x, -lhs.y, -lhs.z };
}

template<typename T1, typename T2>
auto operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs) -> Vec3<decltype(std::declval<T1>() * std::declval<T2>())>
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

template<typename T1, typename T2>
auto operator/(const Vec3<T1>& lhs, const Vec3<T2>& rhs) -> Vec3<decltype(std::declval<T1>() / std::declval<T2>())>
{
	return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
}

template<typename T1, typename T2>
Vec3<T1> operator/(const Vec3<T1>& vec, const T2& val)
{
	return { vec.x / val, vec.y / val, vec.z / val };
}

template<typename T1, typename T2>
Vec3<T1> operator/(const T2& val, const Vec3<T1>& vec)
{
	return { val / vec.x, val / vec.y, val / vec.z };
}

template<typename T1, typename T2>
Vec3<T1> operator*(const Vec3<T1>& vec, const T2& val)
{
	return { vec.x * val, vec.y * val, vec.z * val };
}

template<typename T1, typename T2>
Vec3<T1> operator*(const T2& val, const Vec3<T1>& vec)
{
	return { vec.x * val, vec.y * val, vec.z * val };
}

template<typename T>
struct Vec2 {
	T x, y;

	Vec2(T a, T b) { x = a; y = b; }
	Vec2(T a) { x = a; y = a; }
	Vec2() { x = 0; y = 0; }
	T len() { return sqrt(x * x + y * y); }
	Vec3<T> norm() { float l = q_isqrt(x * x + y * y); return { x * l, y * l }; }
};

template<typename T1, typename T2>
auto operator+(const Vec2<T1>& lhs, const Vec2<T2>& rhs) -> Vec2<decltype(std::declval<T1>() + std::declval<T2>())>
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template<typename T1, typename T2>
auto operator-(const Vec2<T1>& lhs, const Vec2<T2>& rhs) -> Vec2<decltype(std::declval<T1>() - std::declval<T2>())>
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

template<typename T1>
auto operator-(const Vec2<T1>& lhs) -> Vec2<decltype(-std::declval<T1>())>
{
	return { -lhs.x, -lhs.y };
}

template<typename T1, typename T2>
auto operator*(const Vec2<T1>& lhs, const Vec2<T2>& rhs) -> Vec2<decltype(std::declval<T1>()* std::declval<T2>())>
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

template<typename T1, typename T2>
auto operator/(const Vec2<T1>& lhs, const Vec2<T2>& rhs) -> Vec2<decltype(std::declval<T1>() / std::declval<T2>())>
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

template<typename T1, typename T2>
Vec2<T1> operator/(const Vec2<T1>& vec, const T2& val)
{
	return { vec.x / val, vec.y / val };
}

template<typename T1, typename T2>
Vec2<T1> operator/(const T2& val, const Vec2<T1>& vec)
{
	return { val / vec.x, val / vec.y };
}

template<typename T1, typename T2>
Vec2<T1> operator*(const Vec2<T1>& vec, const T2& val)
{
	return { vec.x * val, vec.y * val };
}

template<typename T1, typename T2>
Vec2<T1> operator*(const T2& val, const Vec2<T1>& vec)
{
	return { vec.x * val, vec.y * val };
}


template<typename T>
float dist(const Vec3<T>& a, const Vec3<T>& b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

template<typename T>
float vecDot(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T>
float dist(const Vec2<T>& a, const Vec2<T>& b) {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

template<typename T>
float vecDot(const Vec2<T>& a, const Vec2<T>& b) {
	return a.x * b.x + a.y * b.y;
}


Vec3<float> rotateX(const Vec3<float>& point, float angle);
Vec3<float> rotateY(const Vec3<float>& point, float angle);
Vec3<float> rotateZ(const Vec3<float>& point, float angle);
Vec3<float> rotateXYZ(const Vec3<float>& point, const Vec3<float>& angle);
