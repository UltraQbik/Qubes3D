#pragma once
#include <iostream>
#include <smmintrin.h>
#include <cmath>



// Fast vector XYZ (Vector 3)
struct FVec3
{
    inline FVec3() : mmvalue(_mm_setzero_ps()) {}
    inline FVec3(float x, float y, float z) : mmvalue(_mm_set_ps(0, z, y, x)) {}
    inline FVec3(float x, float y) : mmvalue(_mm_set_ps(0, 0, y, x)) {}
    inline FVec3(float x) : mmvalue(_mm_set_ps(0, x, x, x)) {}
    inline FVec3(__m128 m) : mmvalue(m) {}

    /// Arithmetic operators with FVec3
    inline FVec3 operator+(const FVec3& b) const { return _mm_add_ps(mmvalue, b.mmvalue); }
    inline FVec3 operator-(const FVec3& b) const { return _mm_sub_ps(mmvalue, b.mmvalue); }
    inline FVec3 operator*(const FVec3& b) const { return _mm_mul_ps(mmvalue, b.mmvalue); }
    inline FVec3 operator/(const FVec3& b) const { return _mm_div_ps(mmvalue, b.mmvalue); }

    /// Assignation and arithmetic operators with FVec3
    inline FVec3& operator+=(const FVec3& b) { mmvalue = _mm_add_ps(mmvalue, b.mmvalue); return *this; }
    inline FVec3& operator-=(const FVec3& b) { mmvalue = _mm_sub_ps(mmvalue, b.mmvalue); return *this; }
    inline FVec3& operator*=(const FVec3& b) { mmvalue = _mm_mul_ps(mmvalue, b.mmvalue); return *this; }
    inline FVec3& operator/=(const FVec3& b) { mmvalue = _mm_div_ps(mmvalue, b.mmvalue); return *this; }

    /// Arithmetic operators with floats
    inline FVec3 operator+(float b) const { return _mm_add_ps(mmvalue, _mm_set1_ps(b)); }
    inline FVec3 operator-(float b) const { return _mm_sub_ps(mmvalue, _mm_set1_ps(b)); }
    inline FVec3 operator*(float b) const { return _mm_mul_ps(mmvalue, _mm_set1_ps(b)); }
    inline FVec3 operator/(float b) const { return _mm_div_ps(mmvalue, _mm_set1_ps(b)); }

    /// Assignation and arithmetic operators with float
    inline FVec3& operator+=(float b) { mmvalue = _mm_add_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline FVec3& operator-=(float b) { mmvalue = _mm_sub_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline FVec3& operator*=(float b) { mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(b)); return *this; }
    inline FVec3& operator/=(float b) { mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(b)); return *this; }

    /// Dot product
    inline float dot(const FVec3& b) const { return _mm_cvtss_f32(_mm_dp_ps(mmvalue, b.mmvalue, 0x71)); }
    /// Length of the vector
    inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71))); }
    /// Returns the normalized vector
    inline FVec3 normalize() const {
        // multiplying by rsqrt does not yield an accurate enough result, so we
        // divide by sqrt instead.
        return _mm_div_ps(mmvalue, _mm_sqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0xFF)));
    }

    /// Textual representation
    friend std::ostream& operator<<(std::ostream& os, const FVec3& v)
    {
        os << "Vec3(" << v.x << ", "
            << v.y << ", "
            << v.z << ")";
        return os;
    }

    union
    {
        struct { float x, y, z; };
        __m128 mmvalue;
    };
};

inline FVec3 operator+(float a, const FVec3& b) { return b + a; }
inline FVec3 operator-(float a, const FVec3& b) { return FVec3(_mm_set1_ps(a)) - b; }
inline FVec3 operator*(float a, const FVec3& b) { return b * a; }
inline FVec3 operator/(float a, const FVec3& b) { return FVec3(_mm_set1_ps(a)) / b; }


// Vector XYZ (vector 3)
template<typename T>
struct Vec3 {
	T x, y, z;

	inline Vec3() { x = 0; y = 0; z = 0; }
	inline Vec3(T _x) { x = _x; y = _x; z = _x; }
	inline Vec3(T _x, T _y) { x = _x; y = _y; z = 0; }
	inline Vec3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

	float length() { return sqrtf(x * x + y * y + z * z); }
	Vec3<T> norm() { float iL = 1 / sqrtf(x * x + y * y + z * z); return { (T)(x * iL), (T)(y * iL), (T)(z * iL) }; }
	void normalize() { float iL = 1 / sqrtf(x * x + y * y + z * z); x *= iL; y *= iL; z *= iL; }
};

// -Vec3
template<typename T1>
inline Vec3<T1> operator-(const Vec3<T1>& a) { return { -a.x, -a.y, -a.z }; }

// Arithmetic operators with Vec3
template<typename T1, typename T2>
inline Vec3<T1> operator+(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
template<typename T1, typename T2>
inline Vec3<T1> operator-(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
template<typename T1, typename T2>
inline Vec3<T1> operator*(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
template<typename T1, typename T2>
inline Vec3<T1> operator/(const Vec3<T1>& a, const Vec3<T2>& b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }

// Assignation and arithmetic operators with Vec3
template<typename T1, typename T2>
inline Vec3<T1>& operator+=(const Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator-=(const Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator*=(const Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator/=(const Vec3<T1>& a, const Vec3<T2>& b) { a.x = a.x + b.x; a.y = a.y + b.y; a.z = a.z + b.z; return *this; }

// Arithmetic operators with T2
template<typename T1, typename T2>
inline Vec3<T1> operator+(const Vec3<T1>& a, T2 b) { return { a.x + b, a.y + b, a.z + b }; }
template<typename T1, typename T2>
inline Vec3<T1> operator-(const Vec3<T1>& a, T2 b) { return { a.x - b, a.y - b, a.z - b }; }
template<typename T1, typename T2>
inline Vec3<T1> operator*(const Vec3<T1>& a, T2 b) { return { a.x * b, a.y * b, a.z * b }; }
template<typename T1, typename T2>
inline Vec3<T1> operator/(const Vec3<T1>& a, T2 b) { return { a.x / b, a.y / b, a.z / b }; }

// reverse of that
template<typename T1, typename T2>
inline Vec3<T1> operator+(T2 a, const Vec3<T1>& b) { return b + a; }
template<typename T1, typename T2>
inline Vec3<T1> operator-(T2 a, const Vec3<T1>& b) { return Vec3(a) - b; }
template<typename T1, typename T2>
inline Vec3<T1> operator*(T2 a, const Vec3<T1>& b) { return b * a; }
template<typename T1, typename T2>
inline Vec3<T1> operator/(T2 a, const Vec3<T1>& b) { return Vec3(a) / b; }

// Assignation and arithmetic operators with T2
template<typename T1, typename T2>
inline Vec3<T1>& operator+=(const Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator-=(const Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator*=(const Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return *this; }
template<typename T1, typename T2>
inline Vec3<T1>& operator/=(const Vec3<T1>& a, T2 b) { a.x = a.x + b; a.y = a.y + b; a.z = a.z + b; return *this; }

// Dot product
template<typename T1, typename T2>
inline float dotProduct(const Vec3<T1>& a, const Vec3<T2>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
