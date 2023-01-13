#pragma once
#include <array>
#include <stdlib.h>

#include "configuration.h"
#include "blocks.h"

void generate_empty_map(std::array<std::array<std::array<BLOCK_ID, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z>& _map) {
    // This will generate an empty map

    for (int i = 0; i < g_MAP_SIZE_Z; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    _map[i][j][k] = 0;
}

void generate_debug_map(std::array<std::array<std::array<BLOCK_ID, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z>& _map) {
    // This will generate just a 3D grid of blocks with random id's

    for (int i = 0; i < g_MAP_SIZE_Z; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    _map[i][j][k] = rand() % 256;
}

void generate_flatworld_map(std::array<std::array<std::array<BLOCK_ID, g_MAP_SIZE_X>, g_MAP_SIZE_Y>, g_MAP_SIZE_Z>& _map) {
    // This will generate a simple flatworld, with ground level starting at g_MAP_SIZE_Z / 2 - 1

    for (int i = 0; i < g_MAP_SIZE_Z / 2; i++)
        for (int j = 0; j < g_MAP_SIZE_Y; j++)
            for (int k = 0; k < g_MAP_SIZE_X; k++)
                _map[i][j][k] = 1;
}