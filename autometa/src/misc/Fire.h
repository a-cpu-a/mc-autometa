#pragma once

#include <vector>
#include <random>

struct FireGenerator
{
	size_t xSize = SIZE;//width
	size_t ySize = SIZE;//height

	size_t frame = 0;

	std::vector<float> heatMap = std::vector<float>(xSize * (ySize * 20 / 16), 0.0f);//a0
	//std::vector<float> potMap = std::vector<float>(xSize * ySize, 0.0f);
	//std::vector<float> flameMap = std::vector<float>(xSize * ySize, 0.0f);

	std::vector<uint8_t> img = std::vector<uint8_t>(xSize * ySize * 4, 0);

	std::default_random_engine e;
	std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

	void onTick()
	{
		const size_t pxCount = xSize * ySize;
		const size_t pxDataCount = pxCount*4;
		if (pxDataCount != img.size())
		{
			heatMap.resize(xSize * (ySize * 20 / 16));

			img.resize(pxDataCount);
		}

		const float reducer = 1.0f + 15.36f / (16.0f*ySize);
		const int32_t offset = ySize/16;
		const int32_t int32_0_ = 14 + (offset +1)*(offset +1);
		int32_t times = 0;

		if (offset>=4)
			times = 2;
		else
			times = 1;


		//const float totalWeightR = 1.0 / (float((blurSizeX * 2 + 1) * (blurSizeY * 2 + 1)) * 1.1f);


		const size_t ySizeBuf = ySize * 20 / 16;

		for (size_t t = 0; t < times; t++)
		{
			std::vector<float> nextHeatMap = std::vector<float>(heatMap.size(), 0.0f);

			for (size_t x = 0; x < xSize; x++)
			{
				for (size_t y = 0; y < ySizeBuf; y++)
				{
					size_t _count = int32_0_;


					float sum = heatMap[x+((y+1)% ySizeBuf)*xSize] * float(int32_0_);

					int32_t bx = x - 1;
					const int32_t lsX = x + 1;
					const int32_t lsY = y + 1;

					for (; bx <= lsX; bx++)
					{
						int32_t by = y;
						for (; by <= lsY; by++)
						{
							if (by >= 0 && by < ySizeBuf)
								sum += heatMap[(bx%xSize)+by*xSize];

							_count++;
						}
					}

					if (y>= ySizeBuf -1)
						nextHeatMap[x + y * xSize] = dis(e)* dis(e)* dis(e)*4.0f+ dis(e)*0.1f+0.2f;
					else
						nextHeatMap[x + y * xSize] = sum / (float(_count) * reducer);

				}
			}
			heatMap = nextHeatMap;
		}

		for (size_t i = 0; i < pxCount * 4; i += 4)
		{
			const float val = std::max(std::min(heatMap[i / 4] * 1.8f, 1.0f), 0.0f);

			if (false)
			{
				const int32_t r = int32_t(val * 155.0f + 100.0f);
				const int32_t g = int32_t(val * val * 255.0f);
				const int32_t b = int32_t(val * val * val * val * val * val * val * val * val * val * 255.0f);
				img[i + 0] = uint8_t((r * 30 + g * 59 + b * 11) / 100);
				img[i + 1] = uint8_t((r * 30 + g * 70) / 100);
				img[i + 2] = uint8_t((r * 30 + b * 70) / 100);
				if(val<0.5f)
					img[i + 3] = 0;
				else
					img[i + 3] = UINT8_MAX;
			}
			else
			{
				img[i + 0] = uint8_t(val * 155.0f + 100.0f);
				img[i + 1] = uint8_t(val * val * 255.0f);
				img[i + 2] = uint8_t(val * val * val * val * val * val * val * val * val * val * 255.0f);
				if (val < 0.5f)
					img[i + 3] = 0;
				else
					img[i + 3] = UINT8_MAX;
			}
		}
	}
};