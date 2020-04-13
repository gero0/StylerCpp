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
		void setProportionalPosition(float position);
		float getProportionalPosition();
		size_t getPosition();
		~Instrument();
	private:
		float volume = 0.5f;
		size_t bufferSize;
		std::unordered_map<Chord, std::shared_ptr<Track> > tracks;
		std::shared_ptr<float[]> trackBuffer;
		std::shared_ptr<float[]> temp;
		Chord currentChord = Chord::None;
	};
}


