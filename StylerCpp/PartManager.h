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
		PartManager();
		uint32_t readStream(float* buffer, size_t offset, size_t count);
	private:
		std::unordered_map<std::string, Part> tracks;
		Part* currentPart;
		PaStream *stream;
	};
}

