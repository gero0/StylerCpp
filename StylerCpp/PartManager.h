#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include <mutex>
#include "Part.h"


namespace Styler {
	class Player;

	class PartManager
	{
	public:
		PartManager(size_t bufferSize);
		size_t readStream(float* buffer, size_t offset, size_t count);
		void moveParts(std::unordered_map<std::string, Part> partMap);
		void addPart(std::string partname, Part part);
		void setPart(std::string partName);
		void setVolume(std::string instrument, float volume);
		void setChord(Chord chord);
		void setPosition(size_t position);
		void setProportionalPosition(float position);
		float masterVolume = 1.0f;
		std::unordered_map<std::string, Part>::iterator currentPart;
	private:
		float* sampleBuffer;
		std::mutex accessLock;
		size_t bSize = 0;
		std::unordered_map<std::string, Part> parts;	
		Chord currentChord = Chord::None;

		//temporary
		
	};
}

