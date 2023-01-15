#include "vector.h"


Vec3<float> rotateX(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x, point.y * std::cosf(angle) - point.z * std::sinf(angle), point.z * std::cosf(angle) + point.y * std::sinf(angle));
}

Vec3<float> rotateY(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x * std::cosf(angle) - point.z * std::sinf(angle), point.y, point.z * std::cosf(angle) + point.x * std::sinf(angle));
}

Vec3<float> rotateZ(const Vec3<float>& point, float angle) {
    return Vec3<float>(point.x * std::cosf(angle) - point.y * std::sinf(angle), point.y * std::cosf(angle) + point.x * std::sinf(angle), point.z);
}

Vec3<float> rotateXYZ(const Vec3<float>& point, const Vec3<float>& angle) {
    return rotateZ(rotateY(rotateX(point, angle.x), angle.y), angle.z);
}