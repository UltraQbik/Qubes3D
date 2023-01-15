#pragma once
#include <stdint.h>


typedef unsigned char BLOCK_ID;
typedef char COLOR;


constexpr float g_RENDER_DIST = 110.f;
constexpr float g_AMBIENT = 0.2f;

constexpr float g_CAMERA_SPEED = 12.f;
constexpr float g_CAMERA_SHIFT_MUL = 1.8f;

constexpr float g_MOUSE_SENSITIVITY = 64.f;

constexpr uint16_t g_CHUNK_SIZE_X = 32;
constexpr uint16_t g_CHUNK_SIZE_Y = 32;
constexpr uint16_t g_CHUNK_SIZE_Z = 32;

constexpr uint16_t g_MAP_SIZE_X = 32;
constexpr uint16_t g_MAP_SIZE_Y = 32;
constexpr uint16_t g_MAP_SIZE_Z = 32;