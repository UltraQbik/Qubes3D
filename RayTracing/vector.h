#pragma once
#include <type_traits>
#include <cmath>



template<typename T>
struct Vec3 {
	T x, y, z;

	Vec3(T a, T b, T c) { x = a; y = b; z = c; }
	Vec3(T a) { x = a; y = a; z = a; }
	Vec3() { x = 0; y = 0; z = 0; }
	float len() { return sqrtf(x * x + y * y + z * z); }
	Vec3 norm() { return Vec3(x / this->len(), y / this->len(), z / this->len()); }
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
	float len() { return sqrtf(x * x + y * y); }
	Vec2 norm() { return Vec2(x / this->len(), y / this->len()); }
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
float dist(Vec3<T>& a, Vec3<T>& b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) + (a->z - b->z) * (a->z - b->z));
}

template<typename T>
float dist(Vec2<T>& a, Vec2<T>& b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}


inline Vec3<float> rotate_x(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x, point.y * std::cosf(angle) - point.z * std::sinf(angle), point.z * std::cosf(angle) + point.y * std::sinf(angle));
}

inline Vec3<float> rotate_y(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x * std::cosf(angle) - point.z * std::sinf(angle), point.y, point.z * std::cosf(angle) + point.x * std::sinf(angle));
}

inline Vec3<float> rotate_z(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x * std::cosf(angle) - point.y * std::sinf(angle), point.y * std::cosf(angle) + point.x * std::sinf(angle), point.z);
}

inline Vec3<float> rotate_xyz(const Vec3<float>& point, const Vec3<float>& angle) {
    return rotate_z(rotate_y(rotate_x(point, angle.x), angle.y), angle.z);
}
