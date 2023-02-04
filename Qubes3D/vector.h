#pragma once
#include <iostream>
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

// TODO: transfer operators into struct scope

// -Vec3
template<typename T1>
Vec3<T1> operator-(const Vec3<T1>& a) { return { -a.x, -a.y, -a.z }; }

// Arithmetic operators with Vec3
template<typename T1, typename T2>
Vec3<T1> operator+(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
template<typename T1, typename T2>
Vec3<T1> operator-(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
template<typename T1, typename T2>
Vec3<T1> operator*(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
template<typename T1, typename T2>
Vec3<T1> operator/(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }

// Assignation and arithmetic operators with Vec3
template<typename T1, typename T2>
Vec3<T1>& operator+=(Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator-=(Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator*=(Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator/=(Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return a; }

// Arithmetic operators with T2
template<typename T1, typename T2>
Vec3<T1> operator+(const Vec3<T1>& a, T2 b) { return { a.x + b, a.y + b, a.z + b }; }
template<typename T1, typename T2>
Vec3<T1> operator-(const Vec3<T1>& a, T2 b) { return { a.x - b, a.y - b, a.z - b }; }
template<typename T1, typename T2>
Vec3<T1> operator*(const Vec3<T1>& a, T2 b) { return { a.x * b, a.y * b, a.z * b }; }
template<typename T1, typename T2>
Vec3<T1> operator/(const Vec3<T1>& a, T2 b) { return { a.x / b, a.y / b, a.z / b }; }

// reverse of that
template<typename T1, typename T2>
Vec3<T1> operator+(T2 a, const Vec3<T1>& b) { return b + a; }
template<typename T1, typename T2>
Vec3<T1> operator-(T2 a, const Vec3<T1>& b) { return Vec3(a) - b; }
template<typename T1, typename T2>
Vec3<T1> operator*(T2 a, const Vec3<T1>& b) { return b * a; }
template<typename T1, typename T2>
Vec3<T1> operator/(T2 a, const Vec3<T1>& b) { return Vec3(a) / b; }

// Assignation and arithmetic operators with T2
template<typename T1, typename T2>
Vec3<T1>& operator+=(Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator-=(Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator*=(Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return a; }
template<typename T1, typename T2>
Vec3<T1>& operator/=(Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return a; }

// Textual representation
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
{
	os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

// Dot product
template<typename T1, typename T2>
float dotProduct(const Vec3<T1>& a, const Vec3<T2>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
