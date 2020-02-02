#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include "Part.h"
#include <portaudio.h>
#include <sndfile.h>

namespace Styler {
	class PartManager
	{
	public:
		PartManager(size_t bufferSize);
		size_t readStream(float* buffer, size_t offset, size_t count);
		Instrument ins;
	private:
		size_t bSize = 0;
		std::unordered_map<std::string, Part> tracks;
		Part* currentPart;

		//temporary
		
	};
}

