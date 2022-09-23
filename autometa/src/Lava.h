#pragma once

#include <iostream>

#include <array>

#include "Defines.h"

class Lava
{
private:
	std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> m_pixelData; //[16][16][4]

	float m_soupHeatMap[SIZE][SIZE];
	float m_flameMap[SIZE][SIZE];
	float m_potMap[SIZE][SIZE];

	float m_seed = 1;


public:

	Lava()
	{
		srand(m_seed);
		for (size_t x = 0; x < SIZE; x++)
		{
			for (size_t y = 0; y < SIZE; y++)
			{
				m_soupHeatMap[x][y] = 0.0;
				m_flameMap[x][y] = 0.0;
				m_potMap[x][y] = 0.0;
			}
		}
	}

	~Lava()
	{

	}

	void Tick();

	std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> GetData();
};

