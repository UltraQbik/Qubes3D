#pragma once
#include <vector>
#include "Config.h"
#include "Vector.h"


struct Chunk
{
public:
	Chunk();

	std::vector<BID>& getBlockArray() { return m_BlockArray; }

	BID getBlock(const Vec3<POS>& _pos);
	BID getBlock(POS _x, POS _y, POS _z);
	BID getBlockP(const Vec3<POS>& _pos);
	BID getBlockP(POS _x, POS _y, POS _z);

	void setBlock(const Vec3<POS>& _pos, BID _id);
	void setBlock(POS _x, POS _y, POS _z, BID _id);
	void setBlockP(const Vec3<POS>& _pos, BID _id);
	void setBlockP(POS _x, POS _y, POS _z, BID _id);
private:
	std::vector<BID> m_BlockArray;
};

struct World
{
public:
	World();

	std::vector<Chunk>& getChunkArray() { return m_ChunkArray; }

	Chunk& getChunk(const Vec3<POS>& _pos);
	Chunk& getChunk(POS _x, POS _y, POS _z);
	Chunk& getChunkP(const Vec3<POS>& _pos);
	Chunk& getChunkP(POS _x, POS _y, POS _z);

	BID getBlock(const Vec3<POS>& _pos);
	BID getBlock(POS _x, POS _y, POS _z);
	BID getBlockP(const Vec3<POS>& _pos);
	BID getBlockP(POS _x, POS _y, POS _z);

	void setBlock(const Vec3<POS>& _pos, BID _id);
	void setBlock(POS _x, POS _y, POS _z, BID _id);
	void setBlockP(const Vec3<POS>& _pos, BID _id);
	void setBlockP(POS _x, POS _y, POS _z, BID _id);
private:
	std::vector<Chunk> m_ChunkArray;
};


void generateDebugWorld(World& _world);
void generateFlatWorld(World& _world, const int level);


extern World g_World;
