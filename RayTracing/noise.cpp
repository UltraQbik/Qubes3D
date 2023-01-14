#include <stdlib.h>

#include "noise.h"


NoiseMap2D::NoiseMap2D(uint16_t _x, uint16_t _y) {
	m_NoiseMap = new float[_x * _y];
	m_Size = Vec2<uint16_t>(_x, _y);

	this->GenerateNoise();
}

NoiseMap2D::NoiseMap2D(Vec2<uint16_t> _size) {
	m_NoiseMap = new float[_size.x * _size.y];
	m_Size = _size;

	this->GenerateNoise();
}

void NoiseMap2D::GenerateNoise() {
	// TODO: use some kind of interpolation for much faster noise generation

	// fill the noise map with value 1
	for (uint16_t i = 0; i < m_Size.y; i++)
		for (uint16_t j = 0; j < m_Size.x; j++)
			m_NoiseMap[i + j * m_Size.x] = 1.f;

	for (unsigned char oct = 1; oct <= 5; oct++) 
	{
		// apply random noise to the entire map
		for (uint16_t i = 0; i < m_Size.y; i += oct) {
			for (uint16_t j = 0; j < m_Size.x; j += oct) {
				// generate tiles with 1 uniform randomly defined value
				float r = (float)rand() / RAND_MAX;
				for (uint16_t io = 0; io < oct; io++)
					for (uint16_t jo = 0; jo < oct; jo++)
						m_NoiseMap[(i + io) + (j + jo) * m_Size.x] = (r + m_NoiseMap[(i + io) + (j + jo) * m_Size.x]) / 2.f;
			}
		}
	}
}

float NoiseMap2D::GetNoiseAt(uint16_t _x, uint16_t _y) {
	return m_NoiseMap[_x + _y * m_Size.x];
}

float NoiseMap2D::GetNoiseAt(Vec2<uint16_t> _pos) {
	return m_NoiseMap[_pos.x + _pos.y * m_Size.x];
}

float MathNoise(float _seed) {
	return std::fabsf(std::sinf(_seed * 5444343332.235f));
}
