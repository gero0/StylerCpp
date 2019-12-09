#include "Instrument.h"
#include <iostream>

namespace Styler {

	Instrument::Instrument(size_t bufferSize){
		this->bufferSize = bufferSize;
		sumBuffer = new float[bufferSize];
		trackBuffer = new float[bufferSize];
	}

	void Instrument::setVolume(float volume)
	{
		this->volume = volume;
	}

	void Instrument::addTrack(Chord chord, std::string filePath, int channels, int sampleRate)
	{

		try {
			Track newTrack(filePath, channels, sampleRate);

			auto iter = tracks.find(chord);

			if (iter != tracks.end())
				throw new KeyAlreadyExistsException;

			tracks.insert({ chord, newTrack });
		}
		catch (const InvalidAudioFileException& e) {
			throw e;
		}
		catch (const std::exception& e) {
			throw e;
		}
		
	}

	size_t Instrument::read(float* buffer, size_t count)
	{
		size_t bytesRead;
		//clearing the sum buffer
		std::fill(sumBuffer, sumBuffer + bufferSize, 0);
		for (auto& track : tracks) {
			bytesRead = track.second.read(trackBuffer, bufferSize);
			for (size_t i = 0; i < bytesRead; i++) {
				// (sample * track volume * instrument volume) / number of tracks
				sumBuffer[i] += (trackBuffer[i] * track.second.volume * volume) / tracks.size();
				std::fill(trackBuffer, trackBuffer + bufferSize, 0);
			}
		}

		memcpy(buffer, sumBuffer, sizeof(float) * bytesRead);
		return bytesRead;
	}

	void Instrument::setChord(Chord c)
	{
		for (auto& track : tracks) {
			track.second.volume = 0;
		}

		tracks[c].volume = 1;
	}

	Instrument::~Instrument() {
		delete sumBuffer;
		delete trackBuffer;
	}
}

