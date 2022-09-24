#pragma once

#include <vector>
#include <random>

struct PortalGenerator
{
	size_t xSize = SIZE;//width
	size_t ySize = SIZE;//height

	size_t frame = 0;

	std::vector<std::vector<uint8_t>> frameData;//[frame(32)][pix]

	//std::vector<float> soupHeatMap = std::vector<float>(xSize * ySize, 0.0f);//a0
	//std::vector<float> potMap = std::vector<float>(xSize * ySize, 0.0f);
	//std::vector<float> flameMap = std::vector<float>(xSize * ySize, 0.0f);

	std::vector<uint8_t> img = std::vector<uint8_t>(xSize * ySize * 4, 0);

	std::default_random_engine e;
	std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

	void resetData(const size_t& pixDataCount)
	{
		frameData = std::vector<std::vector<uint8_t>>(32, std::vector<uint8_t>(pixDataCount,0));
		//there are 32 frames

		for (size_t i = 0; i < 32; i++)
		{

			std::vector<uint8_t>& curFrameData = frameData[i];
			const float timeValue = ((float(i) / 32.0f) * float(std::_Pi) * 2.0f);

			for (size_t x = 0; x < xSize; x++)
			{
				for (size_t y = 0; y < ySize; y++)
				{
					float value = 0.0f;

					for (int32_t j = 0; j < 2; j++)
					{
						const float v0_ = float(j * xSize / 2);
						const float v1_ = float(j * ySize / 2);
						float v2_ = (float(x) - v0_) / float(xSize) * 2.0f;
						float v3_ = (float(y) - v1_) / float(ySize) * 2.0f;
						
						if (v2_ < -1.0f)
							v2_ += 2.0f;
						else if (v2_ >= 1.0f)
							v2_ -= 2.0f;

						if (v3_ < -1.0f)
							v3_ += 2.0f;
						else if(v3_ >= 1.0f)
							v3_ -= 2.0f;


						const float mag =  v2_* v2_ + v3_ * v3_;
						const float spiralRot = atan2(v3_, v2_);

						const float spiralIdx = float(j * 2);

						float spiralValue = spiralRot + (timeValue - (mag * 10.0f) + spiralIdx) * (spiralIdx-1.0f);
						spiralValue = (sin(spiralValue) + 1.0f) * 0.5f;
						spiralValue /= mag + 1.0f;
						value += spiralValue * 0.5f;
						
					}
					value += dis(e) * 0.1f;

					curFrameData[(x + y * xSize) * 4 + 0] = uint8_t(value * value * 200.0f + 55.0f);
					curFrameData[(x + y * xSize) * 4 + 1] = uint8_t(value*value*value*value*255.0f);
					curFrameData[(x + y * xSize) * 4 + 2] = uint8_t(value*100.0f + 155.0f);
					curFrameData[(x + y * xSize) * 4 + 3] = uint8_t(value * 100.0f + 155.0f);

				}
			}
		}
	}

	PortalGenerator()
	{
		const size_t pxCount = xSize * ySize;
		const size_t pixDataCount = pxCount * 4;
		resetData(pixDataCount);
	}

	void onTick()
	{
		const size_t pxCount = xSize * ySize;
		const size_t pixDataCount = pxCount*4;
		if (pixDataCount != frameData[0].size())
		{
			//reset data
			resetData(pixDataCount);
			img.resize(pixDataCount);
		}
		frame++;
		const std::vector<uint8_t>& pixArr = frameData[frame%32];//32 frames

		for (size_t i = 0; i < pxCount * 4; i += 4)
		{
			if (false)
			{
				const int32_t r = int32_t(pixArr[i + 0]);
				const int32_t g = int32_t(pixArr[i + 1]);
				const int32_t b = int32_t(pixArr[i + 2]);
				img[i + 0] = uint8_t((r * 30 + g * 59 + b * 11) / 100);
				img[i + 1] = uint8_t((r * 30 + g * 70) / 100);
				img[i + 2] = uint8_t((r * 30 + b * 70) / 100);
				img[i + 3] = pixArr[i + 3];
			}
			else
			{
				img[i + 0] = uint8_t(pixArr[i + 0]);
				img[i + 1] = uint8_t(pixArr[i + 1]);
				img[i + 2] = uint8_t(pixArr[i + 2]);
				img[i + 3] = pixArr[i + 3];
			}
		}
	}
};