#include "Instrument.h"
#include <iostream>
#include <windows.h>

namespace Styler {

	Instrument::Instrument(size_t bufferSize){
		this->bufferSize = bufferSize;
		
		trackBuffer = new float[bufferSize];
	}

	Instrument::Instrument(Instrument&& other) noexcept{
		volume = other.volume;
		bufferSize = other.bufferSize;
		//moving tracks map
		tracks.merge(other.tracks);
		trackBuffer = other.trackBuffer;
		currentChord = other.currentChord;
		other.trackBuffer = nullptr;
	}

	void Instrument::setVolume(float volume)
	{
		this->volume = volume;
	}

	void Instrument::addTrack(Chord chord, std::string filePath, int channels, int sampleRate)
	{
		try {
			auto newTrack = std::make_unique<Track>(filePath, channels, sampleRate);
			auto iter = tracks.find(chord);

			if (iter != tracks.end())
				throw new KeyAlreadyExistsException;

			tracks.insert({chord, std::move(newTrack)});
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
		size_t bytesRead = 0;
		//clearing the buffers
		memset(trackBuffer, 0, sizeof(float) * bufferSize);

		if (tracks.find(currentChord) != tracks.end()) {
			bytesRead = tracks[currentChord]->read(trackBuffer, bufferSize);

			for (size_t i = 0; i < bytesRead; i++) {
				buffer[i] = trackBuffer[i] * volume;
			}
		}	

		if (tracks.find(Chord::Drum) != tracks.end()) {
			bytesRead = tracks[Chord::Drum]->read(trackBuffer, bufferSize);

			for (size_t i = 0; i < bytesRead; i++) {
				buffer[i] = trackBuffer[i] * volume;
			}
		}

		for(auto& track : tracks) {
			if(track.first != currentChord && track.first != Chord::Drum){
				track.second->setPosition(track.second->getPosition() + bytesRead);
			}
		}

		return bytesRead;
	}

	void Instrument::setChord(Chord c)
	{
		/*auto v = volume;
		volume = 0;*/
		currentChord = c;
		//volume = v;
	}

	void Instrument::setPosition(size_t position)
	{
		for (auto& track : tracks) {
			track.second->setPosition(position);
		}
	}

	void Instrument::setProportionalPosition(float position)
	{
		for (auto& track : tracks) {
			track.second->setProportionalPosition(position);
		}
	}

	size_t Instrument::getPosition()
	{
		//if map is empty return 0
		if (tracks.size() < 1)
			return 0;
		/*else return position of the first track
		tracks must be equal length and are played all at once so 
		they should always have the same position*/
		return tracks.begin()->second->getPosition();
	}

	Instrument::~Instrument() {
		delete trackBuffer;
	}
}

