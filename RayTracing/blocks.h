#pragma once
typedef unsigned short BLOCK_ID;

struct Block {
	BLOCK_ID id = 0;				// block id (obviously)
	char tags = 0b00000000;			// block tags

	// block tags made out of:
	// none | none | none | none | none | none | none | is solid

	Block(BLOCK_ID a = 0, char b = 0b00000000) { id = a; tags = b; }
};
