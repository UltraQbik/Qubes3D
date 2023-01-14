#pragma once
#include <stdint.h>

#include "vector.h"


class NoiseMap2D {
public:
	NoiseMap2D(uint16_t _x, uint16_t _y);
	NoiseMap2D(Vec2<uint16_t> _size);

	float* GetNoiseMap() { return m_NoiseMap; }
	const Vec2<uint16_t>& GetNoiseSize() const { return m_Size; }

	float GetNoiseAt(uint16_t _x, uint16_t _y);
	float GetNoiseAt(Vec2<uint16_t> _pos);
private:
	float* m_NoiseMap;

	Vec2<uint16_t> m_Size;

	void GenerateNoise();
};


float MathNoise(float _seed);
