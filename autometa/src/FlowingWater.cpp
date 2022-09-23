#include "FlowingWater.h"



void FlowingWater::Tick()
{
	for (size_t x = 0; x < SIZE; x++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			float localSoupHeatLevel = m_soupHeatMap[(x - 1) % SIZE][y] + m_soupHeatMap[x][y] + m_soupHeatMap[(x + 1) % SIZE][y];
			//std::cout << localSoupHeatLevel << "\n";


			m_soupHeatMap[(x-1) % SIZE][y] = localSoupHeatLevel/3.225f+m_potMap[x][y]*0.8f; // devide by 3.225 for floww

			m_potMap[(x - 1) % SIZE][y] += m_flameMap[x][y];
			if (m_potMap[(x - 1) % SIZE][y] < 0.0f) m_potMap[(x - 1) % SIZE][y] = 0.0f;

			m_flameMap[(x - 1) % SIZE][y] -= 0.1f * 0.05f;
			if ( (float)rand()/RAND_MAX <= 0.05f) m_flameMap[(x - 1) % SIZE][y] = 0.5f * 0.05f;
			

			/*m_soupHeatMap[x][(y - 1) % m_SIZE] = localSoupHeatLevel / 3.225f + m_potMap[x][y] * 0.8f; // devide by 3.225 for floww

			m_potMap[x][(y - 1) % m_SIZE] += m_flameMap[x][y];
			if (m_potMap[x][(y - 1) % m_SIZE] < 0.0f) m_potMap[x][(y - 1) % m_SIZE] = 0.0f;

			m_flameMap[x][(y - 1) % m_SIZE] -= 0.1f * 0.05f;
			if ((float)rand() / RAND_MAX <= 0.05f) m_flameMap[x][(y - 1) % m_SIZE] = 0.5f * 0.05f;*/
		}
	}
}

std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> FlowingWater::GetData()
{

	for (size_t x = 0; x < SIZE; x++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			float colorHeat = std::max<float>(  std::min<float>(   m_soupHeatMap[x][y],  1.0),  0.0);
			colorHeat *= colorHeat;

			m_pixelData[x][y][0] = 32 + colorHeat * 32;
			m_pixelData[x][y][1] = 50 + colorHeat * 64;
			m_pixelData[x][y][2] = 255;
			m_pixelData[x][y][3] = 146 + colorHeat * 50;


		}
	}


	return m_pixelData;
}
