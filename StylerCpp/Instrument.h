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
		Instrument(const Instrument&) = delete;
		Instrument(Instrument&& other) noexcept;
		void setVolume(float volume);
		void addTrack(Chord chord, std::string filePath, int channels=2, int sampleRate=44100);
		size_t read(float* buffer, size_t count);
		void setChord(Chord c);
		void setPosition(size_t position);
		void setProportionalPosition(float position);
		size_t getPosition();
		~Instrument();
	private:
		float volume = 1.0f;
		size_t bufferSize;
		std::unordered_map<Chord, std::unique_ptr<Track> > tracks;
		float* trackBuffer;
		Chord currentChord = Chord::None;
	};
}


