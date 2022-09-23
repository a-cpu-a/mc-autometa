#include "Lava.h"



void Lava::Tick()
{
	for (size_t x = 0; x < SIZE; x++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{

			signed char offsets[2] = {(int)(1.2 * sin(x*22.5* 0.0174532925) ),(int)(1.2 * sin(y * 22.5 * 0.0174532925)) }; //0.0174532925 = pi/180 = deg to rad

			float localSoupHeatLevel = 
				m_soupHeatMap[(x-1 + offsets[0]) % SIZE][(y   + offsets[1]) % SIZE]+ //-10
				m_soupHeatMap[(x    + offsets[0]) % SIZE][(y   + offsets[1]) % SIZE] +//00
				m_soupHeatMap[(x+1 + offsets[0]) % SIZE][(y   + offsets[1]) % SIZE] +//10

				m_soupHeatMap[(x-1 + offsets[0]) % SIZE][(y+1 + offsets[1]) % SIZE] +//-11
				m_soupHeatMap[(x    + offsets[0]) % SIZE][(y+1 + offsets[1]) % SIZE] +//01
				m_soupHeatMap[(x+1 + offsets[0]) % SIZE][(y+1 + offsets[1]) % SIZE]+//11
				
				m_soupHeatMap[(x-1 + offsets[0]) % SIZE][(y-1 + offsets[1]) % SIZE] +//-1-1
				m_soupHeatMap[(x    + offsets[0]) % SIZE][(y-1 + offsets[1]) % SIZE] +//0-1
				m_soupHeatMap[(x+1 + offsets[0]) % SIZE][(y-1 + offsets[1]) % SIZE];//1-1
			float localPotHeatLevel = 
				m_potMap[x][y]+  //00
				m_potMap[(x - 1) % SIZE][y] + //10
				m_potMap[x][(y - 1) % SIZE] + //01
				m_potMap[(x - 1) % SIZE][(y - 1) % SIZE];//11 
			//std::cout << localSoupHeatLevel << "\n";


			m_soupHeatMap[x][y] = localSoupHeatLevel / 9.9 + localPotHeatLevel / 4 * 0.8; // /9.5 instead of /10 for better / brighter lava

			m_potMap[x][y] += m_flameMap[x][y]*0.01f;
			if (m_potMap[x][y] < 0.0f) m_potMap[x][y] = 0.0f;

			m_flameMap[x][y] -= 0.06f;
			if ( (float)rand()/RAND_MAX <= 0.005f) m_flameMap[x][y] = 1.5f;
			

		}
	}
}

std::array<std::array<std::array<unsigned char, 4>, SIZE>, SIZE> Lava::GetData()
{

	for (size_t x = 0; x < SIZE; x++)
	{
		for (size_t y = 0; y < SIZE; y++)
		{
			float colorHeat = std::max<float>(  std::min<float>(   2.0f * m_soupHeatMap[x][y],  1.0),  0.0);
			//std::cout << m_soupHeatMap[x][y] << "\n";
			m_pixelData[x][y][0] = colorHeat * 100.0f + 155.0f;
			colorHeat *= colorHeat;
			m_pixelData[x][y][1] = colorHeat * 255.0f;
			colorHeat *= colorHeat;
			m_pixelData[x][y][2] = colorHeat * 128.0f;
			m_pixelData[x][y][3] = 255;


		}
	}


	return m_pixelData;
}
