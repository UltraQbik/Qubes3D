#pragma once
#include "Vector.h"


/// Rotations
Vec3f rotateX(const Vec3f& point, float angle);
Vec3f rotateY(const Vec3f& point, float angle);
Vec3f rotateZ(const Vec3f& point, float angle);


/// Shader math
template<typename T>
T clamp(T val, T _min = 0.f, T _max = 1.f);

Vec3f clamp(Vec3f val, float _min = 0.f, float _max = 1.f);
float smoothstep(float val, float _min = 0.f, float _max = 1.f, float _t1 = 0.f, float _t2 = 1.f);