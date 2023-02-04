#pragma once
#include <stdint.h>


typedef unsigned char COL;
typedef signed char   BID;
typedef int           POS;

#ifdef _DEBUG
	static constexpr int g_WINDOW_WIDTH  = 480;
	static constexpr int g_WINDOW_HEIGHT = 360;
	static constexpr int g_WINDOW_FPS    = 30;
#else
	static constexpr int g_WINDOW_WIDTH  = 1280;
	static constexpr int g_WINDOW_HEIGHT = 720;
	static constexpr int g_WINDOW_FPS    = 75;
#endif

static constexpr int g_CHUNK_SIZE = 16;
static constexpr int g_CHUNK_RSH  = 4;
static constexpr int g_MAP_SIZE   = 3;

static constexpr float g_CAM_RENDER_DISTANCE = 16.f;
static constexpr float g_CAM_SENSITIVITY     = 64.f;
static constexpr float g_CAM_SPEED           = 12.f;
