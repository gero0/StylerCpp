#pragma once
#include "Track.h"
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>

namespace Styler {
	class Instrument
	{
	public:
		Instrument(size_t bufferSize);
		void setVolume(float volume);
		void addTrack(Chord chord, std::string filePath, int channels=2, int sampleRate=44100);
		size_t read(float* buffer, size_t count);
		void setChord(Chord c);
		void setPosition(size_t position);
		size_t getPosition();
		~Instrument();
	private:
		float volume = 1.0f;
		size_t bufferSize;
		std::unordered_map<Chord, std::unique_ptr<Track> > tracks;
		float* sumBuffer;
		float* trackBuffer;
		//Change to chord Drum later
		Chord currentChord = Chord::C;
	};
}


