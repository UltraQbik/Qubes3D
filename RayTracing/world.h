#pragma once
#include "configuration.h"
#include "vector.h"


class Chunk {
public:
    Chunk(BLOCK_ID _fill);

    BLOCK_ID* GetAllBlocks() { return m_BlockArray; }

    BLOCK_ID GetBlockAt(Vec3<uint16_t> _pos);
    BLOCK_ID GetBlockAt(uint16_t _x, uint16_t _y, uint16_t _z);

    void SetBlockAt(Vec3<uint16_t> _pos, BLOCK_ID _id);
    void SetBlockAt(uint16_t _x, uint16_t _y, uint16_t _z, BLOCK_ID _id);
private:
    BLOCK_ID* m_BlockArray;
};


void generate_empty_chunk(Chunk& _chunk);
void generate_debug_chunk(Chunk& _chunk);
void generate_flat_chunk(Chunk& _chunk);