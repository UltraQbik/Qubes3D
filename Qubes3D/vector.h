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

	// Arithmetic operators with Vec3
	template<typename To>
	Vec3<T> operator+(const Vec3<To>& o) const { return { this->x + o.x, this->y + o.y, this->z + o.z }; }
	template<typename To>
	Vec3<T> operator-(const Vec3<To>& o) const { return { this->x - o.x, this->y - o.y, this->z - o.z }; }
	template<typename To>
	Vec3<T> operator*(const Vec3<To>& o) const { return { this->x * o.x, this->y * o.y, this->z * o.z }; }
	template<typename To>
	Vec3<T> operator/(const Vec3<To>& o) const { return { this->x / o.x, this->y / o.y, this->z / o.z }; }

	// Assignation and arithmetic operators with Vec3
	template<typename To>
	Vec3<T>& operator+=(const Vec3<To>& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
	template<typename To>
	Vec3<T>& operator-=(const Vec3<To>& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }
	template<typename To>
	Vec3<T>& operator*=(const Vec3<To>& o) { this->x *= o.x; this->y *= o.y; this->z *= o.z; return *this; }
	template<typename To>
	Vec3<T>& operator/=(const Vec3<To>& o) { this->x /= o.x; this->y /= o.y; this->z /= o.z; return *this; }

	// Arithmetic operators with To
	template<typename To>
	Vec3<T> operator+(To o) const { return { this->x + o, this->y + o, this->z + o }; }
	template<typename To>
	Vec3<T> operator-(To o) const { return { this->x - o, this->y - o, this->z - o }; }
	template<typename To>
	Vec3<T> operator*(To o) const { return { this->x * o, this->y * o, this->z * o }; }
	template<typename To>
	Vec3<T> operator/(To o) const { return { this->x / o, this->y / o, this->z / o }; }

	// Assignation and arithmetic operators with T2
	template<typename To>
	Vec3<T>& operator+=(To o) { this->x += o; this->y += o; this->z += o; return *this; }
	template<typename To>
	Vec3<T>& operator-=(To o) { this->x -= o; this->y -= o; this->z -= o; return *this; }
	template<typename To>
	Vec3<T>& operator*=(To o) { this->x *= o; this->y *= o; this->z *= o; return *this; }
	template<typename To>
	Vec3<T>& operator/=(To o) { this->x /= o; this->y /= o; this->z /= o; return *this; }

	// Dot product
	template<typename To>
	T dot(const Vec3<To>& o) const { return this->x * o.x + this->y * o.y + this->z * o.z; }

	// Length of the vector
	T length() const { return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }

	// Vector divided by it's length (unit vector)
	Vec3<T> norm() const { T iL = 1 / this->length(); return *this * iL; }

	// Textual representation
	friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) { os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")"; return os; }

};

// -Vec3
template<typename T>
Vec3<T> operator-(const Vec3<T>& a) { return { -a.x, -a.y, -a.z }; }

// Arithmetic operators with To (reverse)
template<typename T1, typename T2>
Vec3<T1> operator+(T2 a, const Vec3<T1>& b) { return b + a; }
template<typename T1, typename T2>
Vec3<T1> operator-(T2 a, const Vec3<T1>& b) { return Vec3<T1>(a) - b; }
template<typename T1, typename T2>
Vec3<T1> operator*(T2 a, const Vec3<T1>& b) { return b * a; }
template<typename T1, typename T2>
Vec3<T1> operator/(T2 a, const Vec3<T1>& b) { return Vec3<T1>(a) / b; }

// Typedefs
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
