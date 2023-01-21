#pragma once
#include <vector>

#include "Config.h"
#include "Vector.h"


struct Chunk
{
public:
	Chunk();

	BID getBlock(Vec3<POS>& _pos);
	BID getBlock(POS _x, POS _y, POS _z);

	void setBlock(Vec3<POS>& _pos, BID _id);
	void setBlock(POS _x, POS _y, POS _z, BID _id);
private:
	std::vector<BID> m_BlockArray;
};

struct World
{
public:
	World();

	Chunk& getChunk(Vec3<POS>& _pos);
	Chunk& getChunk(POS _x, POS _y, POS _z);

	BID getBlock(Vec3<POS>& _pos);
	BID getBlock(POS _x, POS _y, POS _z);

	void setBlock(Vec3<POS>& _pos, BID _id);
	void setBlock(POS _x, POS _y, POS _z, BID _id);
private:
	std::vector<Chunk> m_ChunkArray;
};
