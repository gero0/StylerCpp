#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include "Track.h"

namespace Styler {
	class TrackManager
	{
	public:
		uint32_t readStream(float* buffer, size_t offset, size_t count);
	private:
		std::unordered_map<std::string, Part> tracks;
		Part* currentTrack;
	};
}

