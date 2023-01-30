#include "World.h"


// World chunks
Chunk::Chunk()
{
	m_BlockArray.resize((uint64_t)g_CHUNK_SIZE * g_CHUNK_SIZE * g_CHUNK_SIZE);
}

BID Chunk::getBlock(Vec3<POS>& _pos)
{
	return m_BlockArray[_pos.x + _pos.y * g_CHUNK_SIZE + _pos.z * g_CHUNK_SIZE * g_CHUNK_SIZE];
}

BID Chunk::getBlock(POS _x, POS _y, POS _z)
{
	return m_BlockArray[_x + _y * g_CHUNK_SIZE + _z * g_CHUNK_SIZE * g_CHUNK_SIZE];
}

void Chunk::setBlock(Vec3<POS>& _pos, BID _id)
{
	m_BlockArray[_pos.x + _pos.y * g_CHUNK_SIZE + _pos.z * g_CHUNK_SIZE * g_CHUNK_SIZE] = _id;
}

void Chunk::setBlock(POS _x, POS _y, POS _z, BID _id)
{
	m_BlockArray[_x + _y * g_CHUNK_SIZE + _z * g_CHUNK_SIZE * g_CHUNK_SIZE] = _id;
}


// The World
World::World()
{
	m_ChunkArray.resize((uint64_t)g_MAP_SIZE * g_MAP_SIZE * g_MAP_SIZE);
}

Chunk& World::getChunk(Vec3<POS>& _pos)
{
	return m_ChunkArray[((POS)_pos.x >> g_CHUNK_RSH) + ((POS)_pos.y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_pos.z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
}

Chunk& World::getChunk(POS _x, POS _y, POS _z)
{
	return m_ChunkArray[((POS)_x >> g_CHUNK_RSH) + ((POS)_y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
}

BID World::getBlock(Vec3<POS>& _pos)
{
	Chunk& chunk = m_ChunkArray[((POS)_pos.x >> g_CHUNK_RSH) + ((POS)_pos.y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_pos.z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
	return chunk.getBlock(_pos.x % g_CHUNK_SIZE, _pos.y % g_CHUNK_SIZE, _pos.z % g_CHUNK_SIZE);
}

BID World::getBlock(POS _x, POS _y, POS _z)
{
	Chunk& chunk = m_ChunkArray[((POS)_x >> g_CHUNK_RSH) + ((POS)_y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
	return chunk.getBlock(_x % g_CHUNK_SIZE, _y % g_CHUNK_SIZE, _z % g_CHUNK_SIZE);
}

void World::setBlock(Vec3<POS>& _pos, BID _id)
{
	Chunk& chunk = m_ChunkArray[((POS)_pos.x >> g_CHUNK_RSH) + ((POS)_pos.y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_pos.z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
	chunk.setBlock(_pos.x % g_CHUNK_SIZE, _pos.y % g_CHUNK_SIZE, _pos.z % g_CHUNK_SIZE, _id);
}

void World::setBlock(POS _x, POS _y, POS _z, BID _id)
{
	Chunk& chunk = m_ChunkArray[((POS)_x >> g_CHUNK_RSH) + ((POS)_y >> g_CHUNK_RSH) * g_MAP_SIZE + ((POS)_z >> g_CHUNK_RSH) * g_MAP_SIZE * g_MAP_SIZE];
	chunk.setBlock(_x % g_CHUNK_SIZE, _y % g_CHUNK_SIZE, _z % g_CHUNK_SIZE, _id);
}



void generateDebugWorld(World& _world)
{
	for (POS i = 0; i < g_CHUNK_SIZE * g_MAP_SIZE; i++)
		for (POS j = 0; j < g_CHUNK_SIZE * g_MAP_SIZE; j++)
			for (POS k = 0; k < g_CHUNK_SIZE * g_MAP_SIZE; k++)
				if (i % 4 == 0 && j % 4 == 0 && k % 4 == 0)
					_world.setBlock(k, j, i, 1);
}
