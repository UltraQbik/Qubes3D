#pragma once
#include <array>


template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
void generate_debug_map(std::array<std::array<std::array<T, N1>, N2>, N3>& world) {
    // This will generate just a 3D grid of blocks with random id's

    for (int i = 0; i < world.size(); i++)
        for (int j = 0; j < world[0].size(); j++)
            for (int k = 0; k < world[0][0].size(); k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    world[i][j][k] = rand() % 256;
}

template <typename T, std::size_t N1, std::size_t N2, std::size_t N3>
void generate_flatworld_map(std::array<std::array<std::array<T, N1>, N2>, N3>& world) {
    // This will generate a simple flatworld, with ground level starting at g_MAP_SIZE_Z / 2 - 1

    for (int i = 0; i < world.size() / 2; i++)
        for (int j = 0; j < world[0].size(); j++)
            for (int k = 0; k < world[0][0].size(); k++)
                world[i][j][k] = 1;
}
