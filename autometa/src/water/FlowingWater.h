#pragma once

#include <vector>
#include <random>

struct FlowingWaterGenerator
{
	size_t xSize = SIZE;//width
	size_t ySize = SIZE;//height

	size_t frame = 0;

	std::vector<float> soupHeatMap = std::vector<float>(xSize * ySize, 0.0f);//a0
	std::vector<float> potMap = std::vector<float>(xSize * ySize, 0.0f);
	std::vector<float> flameMap = std::vector<float>(xSize * ySize, 0.0f);

	std::vector<uint8_t> img = std::vector<uint8_t>(xSize * ySize * 4, 0);

	std::default_random_engine e;
	std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

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

		const int32_t blurSizeY = (int32_t)sqrt(double(ySize / 16));
		frame += blurSizeY;

		const float totalWeightR = 1.0 / (float(blurSizeY * 2 + 1) * 1.0667f);

		std::vector<float> nextSoupHeatMap = std::vector<float>(pxCount, 0.0f);

		for (size_t x = 0; x < xSize; x++)
		{
			for (size_t y = 0; y < ySize; y++)
			{

				float float_0_ = 0.0f;
				int32_t by = y - 2*blurSizeY;

				//const int32_t lsY = y;

				for (; by <= int32_t(y); by++)
				{
					float_0_ += soupHeatMap[x + (by%ySize) * xSize];
				}


				float& potValue = potMap[x + y * xSize];
				float& flameValue = flameMap[x + y * xSize];

				nextSoupHeatMap[x + y * xSize] = float_0_ * totalWeightR + potValue * 0.8f;

				potValue += flameValue*0.05f;
				potValue = std::max(potValue, 0.0f);

				if (dis(e) < 0.2f)
					flameValue = 0.5f;
				else
					flameValue -= 0.3f;

			}
		}
		soupHeatMap = nextSoupHeatMap;

		for (size_t i = 0; i < pxCount * 4; i += 4)
		{
			const float val = std::max(std::min(soupHeatMap[((i / 4) - frame * xSize) % pxCount], 1.0f), 0.0f);

			const float valPow2 = val * val;

			if (false)
			{
				const int32_t r = int32_t(valPow2 * 32.0f + 32.0f);
				const int32_t g = int32_t(valPow2 * 50.0f + 64.0f);
				const int32_t b = 255;
				img[i + 0] = uint8_t((r * 30 + g * 59 + b * 11) / 100);
				img[i + 1] = uint8_t((r * 30 + g * 70) / 100);
				img[i + 2] = uint8_t((r * 30 + b * 70) / 100);
				img[i + 3] = uint8_t(valPow2 * 146.0f + 50.0f);
			}
			else
			{
				img[i + 0] = uint8_t(valPow2*32.0f + 32.0f);
				img[i + 1] = uint8_t(valPow2*50.0f + 64.0f);
				img[i + 2] = 255;
				img[i + 3] = uint8_t(valPow2 * 146.0f + 50.0f);
			}
		}
	}
};