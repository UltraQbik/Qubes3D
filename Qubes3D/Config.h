#pragma once
#include <stdint.h>


typedef unsigned char COL;
typedef signed char   BID;
typedef int           POS;

constexpr int g_CHUNK_SIZE = 16;
constexpr int g_CHUNK_RSH  = 4;
constexpr int g_MAP_SIZE   = 8;

constexpr float g_CAM_RENDER_DISTANCE = 256.f;
constexpr float g_CAM_SENSITIVITY     = 64.f;
constexpr float g_CAM_SPEED           = 12.f;
