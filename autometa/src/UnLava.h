#pragma once

#include <iostream>

#include <array>

#include "Defines.h"

class UnLava
{
private:
	const int m_SIZE = 16;
	std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> m_pixelData; //[16][16][4]

	float m_soupHeatMap[SIZE][SIZE];
	float m_flameMap[SIZE][SIZE];
	float m_potMap[SIZE][SIZE];

	float m_seed = 1;


public:

	UnLava()
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

	~UnLava()
	{

	}

	void Tick();

	std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> GetData();
};

