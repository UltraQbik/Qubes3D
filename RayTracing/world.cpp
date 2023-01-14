#include <stdlib.h>

#include "world.h"


Chunk::Chunk(BLOCK_ID _fill) {
    m_BlockArray = new BLOCK_ID[g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y * g_CHUNK_SIZE_Z];

    for (uint16_t i = 0; i < g_CHUNK_SIZE_Z; i++)
        for (uint16_t j = 0; j < g_CHUNK_SIZE_Y; j++)
            for (uint16_t k = 0; k < g_CHUNK_SIZE_X; k++)
                m_BlockArray[k + j * g_CHUNK_SIZE_X + i * g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y] = _fill;
}

BLOCK_ID Chunk::GetBlockAt(Vec3<uint16_t> _pos) {
    if (_pos.x > 0 && _pos.x < g_CHUNK_SIZE_X && _pos.y > 0 && _pos.y < g_CHUNK_SIZE_Y && _pos.z > 0 && _pos.z < g_CHUNK_SIZE_Z)
        return m_BlockArray[_pos.x + _pos.y * g_CHUNK_SIZE_X + _pos.z * g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y];
    return 0;
}

BLOCK_ID Chunk::GetBlockAt(uint16_t _x, uint16_t _y, uint16_t _z) {
    if (_x > 0 && _x < g_CHUNK_SIZE_X && _y > 0 && _y < g_CHUNK_SIZE_Y && _z > 0 && _z < g_CHUNK_SIZE_Z)
        return m_BlockArray[_x + _y * g_CHUNK_SIZE_X + _z * g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y];
    return 0;
}

void Chunk::SetBlockAt(Vec3<uint16_t> _pos, BLOCK_ID _id) {
    if (_pos.x > 0 && _pos.x < g_CHUNK_SIZE_X && _pos.y > 0 && _pos.y < g_CHUNK_SIZE_Y && _pos.z > 0 && _pos.z < g_CHUNK_SIZE_Z)
        m_BlockArray[_pos.x + _pos.y * g_CHUNK_SIZE_X + _pos.z * g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y] = _id;
}

void Chunk::SetBlockAt(uint16_t _x, uint16_t _y, uint16_t _z, BLOCK_ID _id) {
    if (_x > 0 && _x < g_CHUNK_SIZE_X && _y > 0 && _y < g_CHUNK_SIZE_Y && _z > 0 && _z < g_CHUNK_SIZE_Z)
        m_BlockArray[_x + _y * g_CHUNK_SIZE_X + _z * g_CHUNK_SIZE_X * g_CHUNK_SIZE_Y] = _id;
}

void generateEmptyChunk(Chunk& _chunk) {
    for (uint16_t i = 0; i < g_CHUNK_SIZE_Z; i++)
        for (uint16_t j = 0; j < g_CHUNK_SIZE_Y; j++)
            for (uint16_t k = 0; k < g_CHUNK_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    _chunk.SetBlockAt(k, j, i, 0);
}

void generateDebugChunk(Chunk& _chunk) {
    for (uint16_t i = 0; i < g_CHUNK_SIZE_Z; i++)
        for (uint16_t j = 0; j < g_CHUNK_SIZE_Y; j++)
            for (uint16_t k = 0; k < g_CHUNK_SIZE_X; k++)
                if (i % 8 == 0 && j % 8 == 0 && k % 8 == 0)
                    _chunk.SetBlockAt(k, j, i, rand() % 256);
}

void generateFlatChunk(Chunk& _chunk) {
    for (uint16_t i = 0; i < g_CHUNK_SIZE_Z / 2; i++)
        for (uint16_t j = 0; j < g_CHUNK_SIZE_Y; j++)
            for (uint16_t k = 0; k < g_CHUNK_SIZE_X; k++)
                _chunk.SetBlockAt(k, j, i, 1);
}