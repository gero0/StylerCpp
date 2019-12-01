#pragma once
#include "Instrument.h"
#include <vector>
#include <cstdint>

namespace Styler{
	class Part
	{
	public:
		size_t readStream(float* buffer, size_t offset, size_t count);
		Part(size_t bufferSize);
		~Part();
	private:
		size_t position = 0;
		size_t length = 0;
		bool looped = false; //zast¹piæ typem œcie¿ki
		float* instrumentBuffer;
		std::vector<Instrument> instruments;
	};
};


