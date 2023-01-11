#pragma once
#include <math.h>
#include <iostream>


template<typename T>
struct Vec3 {
	T x, y, z;

	Vec3(T a = 0.0f, T b = 0.0f, T c = 0.0f) { x = a; y = b; z = c; }
	float len() { return sqrtf(x * x + y * y + z * z); }
	Vec3 norm() { return Vec3(x / this->len(), y / this->len(), z / this->len()); }
};

template<typename T>
struct Vec2 {
	T x, y;

	Vec2(T a = 0.0f, T b = 0.0f) { x = a; y = b; }
	float len() { return sqrtf(x * x + y * y); }
	Vec2 norm() { return Vec2(x / this->len(), y / this->len()); }
};

template<typename T>
float dist(Vec3<T>& a, Vec3<T>& b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) + (a->z - b->z) * (a->z - b->z));
}

template<typename T>
float dist(Vec2<T>& a, Vec2<T>& b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}
