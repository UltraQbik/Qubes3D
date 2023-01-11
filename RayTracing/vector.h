#pragma once
#include <math.h>
#include <iostream>


struct Vec3 {
	float x, y, z;

	Vec3(float a = 0.0f, float b = 0.0f, float c = 0.0f) { x = a; y = b; z = c; }
	float len() { return sqrtf(x * x + y * y + z * z); }
	Vec3 norm() { return Vec3(x / this->len(), y / this->len(), z / this->len()); }
	
	void print() { std::cout << x << " | " << y << " | " << z << "\n"; }
};

struct Vec2 {
	float x, y;

	Vec2(float a = 0.0f, float b = 0.0f) { x = a; y = b; }
	float len() { return sqrtf(x * x + y * y); }
	Vec2 norm() { return Vec2(x / this->len(), y / this->len()); }

	void print() { std::cout << x << " | " << y << "\n"; }
};

float dist(Vec3* a, Vec3* b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) + (a->z - b->z) * (a->z - b->z));
}

float dist(Vec2* a, Vec2* b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}
