#pragma once

#include <vector>
#include <random>

struct FlowingLavaGenerator
{
	size_t xSize = SIZE;//width
	size_t ySize = SIZE;//height

	size_t frame = 0;

	std::vector<float> soupHeatMap = std::vector<float>(xSize*ySize,0.0f);//a0
	std::vector<float> potMap = std::vector<float>(xSize * ySize, 0.0f);
	std::vector<float> flameMap = std::vector<float>(xSize * ySize, 0.0f);

	std::vector<uint8_t> img = std::vector<uint8_t>(xSize * ySize*4, 0);

	std::default_random_engine e;
	std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0.0f,1.0f);

	void onTick()
	{
		const size_t pxCount = xSize * ySize;
		if (pxCount != soupHeatMap.size())
		{
			soupHeatMap.resize(pxCount);
			potMap.resize(pxCount);
			flameMap.resize(pxCount);

			img.resize(pxCount * 4);
		}

		const int32_t blurSizeX = (int32_t)sqrt(double(xSize/16));
		const int32_t blurSizeY = (int32_t)sqrt(double(ySize/16));

		frame += blurSizeX;

		const float totalWeightR = 1.0 / (float((blurSizeX * 2 + 1) * (blurSizeY * 2 + 1)) * 1.1f);

		std::vector<float> nextSoupHeatMap = std::vector<float>(pxCount,0.0f);

		for (size_t x = 0; x < xSize; x++)
		{
			for (size_t y = 0; y < ySize; y++)
			{
				int32_t offsetX = (int32_t)(1.2f * sin((double)x * 22.5f * 0.0174532925f));
				int32_t offsetY = (int32_t)(1.2f * sin((double)y * 22.5f * 0.0174532925f));

				float float_0_ = 0.0f;
				int32_t bx = x - blurSizeX + offsetX;

				const int32_t lsX = x + blurSizeX + offsetX;
				const int32_t lsY = y + blurSizeY + offsetY;

				for (; bx <= lsX; bx++)
				{
					int32_t by = y - blurSizeY + offsetY;
					for (; by <= lsY; by++)
					{
						float_0_ += soupHeatMap[(bx % xSize) + (by % ySize) * xSize];
					}

				}

				
				nextSoupHeatMap[x + y * xSize] = float_0_ * totalWeightR + ((
					potMap[((x + 0) % xSize) + ((y + 0) % ySize) * xSize] +
					potMap[((x + 1) % xSize) + ((y + 0) % ySize) * xSize] +
					potMap[((x + 1) % xSize) + ((y + 1) % ySize) * xSize] +
					potMap[((x + 0) % xSize) + ((y + 1) % ySize) * xSize]
					) * 0.2f);

				float& potValue = potMap[x + y * xSize];

				potValue += flameMap[x + y * xSize] *0.01f;
				potValue = std::max(potValue, 0.0f);


				float& flameValue = flameMap[x + y * xSize];

				if (dis(e) < 0.005f)
					flameValue = 1.5f;
				else
					flameValue -= 0.06f;

				



			}
		}
		soupHeatMap = nextSoupHeatMap;

		for (size_t i = 0; i < pxCount*4; i+=4)
		{
			const float val = std::max(std::min(soupHeatMap[((i / 4)-frame / 3 * xSize)% pxCount] * 2.0f,1.0f),0.0f);

			if (false)
			{
				const int32_t r = int32_t(val * 100.0f + 155.0f);
				const int32_t g = int32_t(val * val * 255.0f);
				const int32_t b = int32_t(val * val * val * val * 128.0f);
				img[i + 0] = uint8_t((r * 30 + g * 59 + b * 11) / 100);
				img[i + 1] = uint8_t((r * 30 + g * 70) / 100);
				img[i + 2] = uint8_t((r * 30 + b * 70) / 100);
				img[i + 3] = UINT8_MAX;
			}
			else
			{
				img[i + 0] = uint8_t(val * 100.0f + 155.0f);
				img[i + 1] = uint8_t(val * val * 255.0f);
				img[i + 2] = uint8_t(val * val * val * val * 128.0f);
				img[i + 3] = UINT8_MAX;
			}
		}
	}
};