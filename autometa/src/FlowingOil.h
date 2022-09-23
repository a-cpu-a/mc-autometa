#pragma once

#include <iostream>

#include <array>

class FlowingOil
{
private:
	const int m_SIZE = 16;
	std::array<std::array<std::array<unsigned char, 4>,  16>,  16> m_pixelData; //[16][16][4]

	float m_soupHeatMap[16][16] = { 0 };
	float m_flameMap[16][16] = { 0 };
	float m_potMap[16][16] = { 0 };

	float m_seed = 1;


public:

	FlowingOil()
	{
		srand(m_seed);
		for (size_t x = 0; x < m_SIZE; x++)
		{
			for (size_t y = 0; y < m_SIZE; y++)
			{
				m_soupHeatMap[x][y] = 0.0;
				m_flameMap[x][y] = 0.0;
				m_potMap[x][y] = 0.0;
			}
		}
	}

	~FlowingOil()
	{

	}

	void Tick();

	std::array<std::array<std::array<unsigned char, 4>, 16>, 16> GetData();
};

