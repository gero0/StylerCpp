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
		void addPart(std::string partname, Part part);
		void setPart(std::string partName);
		void setVolume(std::string instrument, float volume);
		void setChord(Chord chord);
		void setPosition(size_t position);
		void setProportionalPosition(float position);
		std::vector<std::string> getInstrumentNames();
	private:
		size_t bSize = 0;
		std::unordered_map<std::string, Part> parts;
		Part* currentPart;

		//temporary
		
	};
}

