#pragma once
#include "Track.h"
#include "Enums.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Styler {
	class Instrument
	{
	public:
		Instrument(size_t bufferSize);
		void setVolume(float volume);
		void addTrack(Chord chord, std::string filePath, int channels=2, int sampleRate=44100);
		size_t read(float* buffer, size_t count);
		void setChord(Chord c);
		~Instrument();
	private:
		float volume = 1;
		size_t bufferSize;
		std::unordered_map<Chord, Track> tracks;
		float* sumBuffer;
		float* trackBuffer;
		Chord currentChord = Chord::None;
	};
}


