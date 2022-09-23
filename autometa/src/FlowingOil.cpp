#include "FlowingOil.h"



void FlowingOil::Tick()
{
	for (size_t x = 0; x < m_SIZE; x++)
	{
		for (size_t y = 0; y < m_SIZE; y++)
		{
			float localSoupHeatLevel = m_soupHeatMap[(x - 1) % m_SIZE][y] + m_soupHeatMap[x][y] + m_soupHeatMap[(x + 1) % m_SIZE][y];
			//std::cout << localSoupHeatLevel << "\n";

			
			m_soupHeatMap[(x-1) % m_SIZE][y] = localSoupHeatLevel/3.225f+m_potMap[x][y]*0.8f; // devide by 3.225 for floww

			m_potMap[(x - 1) % m_SIZE][y] += m_flameMap[x][y];
			if (m_potMap[(x - 1) % m_SIZE][y] < 0.0f) m_potMap[(x - 1) % m_SIZE][y] = 0.0f;

			m_flameMap[(x - 1) % m_SIZE][y] -= 0.1f * 0.05f;
			if ( (float)rand()/RAND_MAX <= 0.05f) m_flameMap[(x - 1) % m_SIZE][y] = 0.5f * 0.05f;
			/*
			m_soupHeatMap[x][(y - 1) % m_SIZE] = localSoupHeatLevel / 3.225f + m_potMap[x][y] * 0.8f; // devide by 3.225 for floww

			m_potMap[x][(y - 1) % m_SIZE] += m_flameMap[x][y];
			if (m_potMap[x][(y - 1) % m_SIZE] < 0.0f) m_potMap[x][(y - 1) % m_SIZE] = 0.0f;

			m_flameMap[x][(y - 1) % m_SIZE] -= 0.1f * 0.05f;
			if ((float)rand() / RAND_MAX <= 0.05f) m_flameMap[x][(y - 1) % m_SIZE] = 0.5f * 0.05f;
			*/
		}
	}
}

std::array<std::array<std::array<unsigned char, 4>, 16>, 16> FlowingOil::GetData()
{

	for (size_t x = 0; x < m_SIZE; x++)
	{
		for (size_t y = 0; y < m_SIZE; y++)
		{
			float colorHeat = std::max<float>(  std::min<float>(   m_soupHeatMap[x][y],  1.0),  0.0);
			colorHeat *= colorHeat;

			m_pixelData[x][y][0] = colorHeat * 47;
			m_pixelData[x][y][1] = colorHeat * 50;
			m_pixelData[x][y][2] = colorHeat * 45;
			m_pixelData[x][y][3] = 255;


		}
	}


	return m_pixelData;
}
