#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
#include <mutex>
#include "Part.h"
#include "Metronome.h"

namespace Styler {
	using pMap = std::unordered_map<std::string, Part>;

	class Player;

	class PartManager
	{
	public:
		PartManager(size_t bufferSize);
		size_t readStream(float* buffer, size_t offset, size_t count);
		void addParts(pMap partMap);
		void addPart(std::string partname, Part part);
		void setPart(std::string partName, float position = 0);
		void setVolume(std::string instrument, float volume);
		void setChord(Chord chord);
		void setPosition(size_t position);
		void setProportionalPosition(float position);
		void changePart(std::string partName, bool isPlaying, Metronome* metro, int metrum);

		float masterVolume = 0.5f;
		pMap::iterator currentPart;
		std::string nextPart = "";
	private:
		pMap parts;
		float* sampleBuffer;
		//locks access to buffer. Prevents switching tracks during reading buffer
		std::mutex accessLock;
		size_t bufferSize = 0;
		Chord currentChord = Chord::None;

		//temporary
	};
}
