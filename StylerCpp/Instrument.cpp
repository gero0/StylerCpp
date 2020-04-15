#include "Instrument.h"
#include <iostream>
#include <windows.h>

namespace Styler {
	Instrument::Instrument(size_t bufferSize) {
		this->bufferSize = bufferSize;

		trackBuffer = { new float[bufferSize], std::default_delete<float[]>() };
		temp = { new float[bufferSize], std::default_delete<float[]>() };
	}

	void Instrument::setVolume(float volume)
	{
		this->volume = volume;
	}

	void Instrument::addTrack(Chord chord, std::string filePath, int channels, int sampleRate)
	{
		try {
			auto newTrack = std::make_shared<Track>(filePath, channels, sampleRate);
			auto iter = tracks.find(chord);

			if (iter != tracks.end())
				throw new KeyAlreadyExistsException;

			tracks.insert({ chord, newTrack });
		}
		catch (const InvalidAudioFileException & e) {
			throw e;
		}
		catch (const std::exception& e) {
			throw e;
		}
	}

	size_t Instrument::read(float* buffer, size_t count)
	{
		size_t samplesRead = 0;

		//clearing the buffers
		std::fill(trackBuffer.get(), trackBuffer.get() + count - 1, 0);

		//Because the drum track is supposed to play no matter which chord is selected, we need to check
		//if this instrument contains either current chord track or drum track
		if (currentChord != Chord::Drum && tracks.find(currentChord) != tracks.end()) {
			samplesRead = tracks[currentChord]->read(trackBuffer.get(), bufferSize);

			for (size_t i = 0; i < samplesRead; i++) {
				buffer[i] = trackBuffer.get()[i] * volume;
			}
		}

		else if (tracks.find(Chord::Drum) != tracks.end()) {
			samplesRead = tracks[Chord::Drum]->read(trackBuffer.get(), bufferSize);

			for (size_t i = 0; i < samplesRead; i++) {
				buffer[i] = trackBuffer.get()[i] * volume;
			}
		}

		//read the same amout of samples from other files to temporary buffer
		//just to keep them synchronised
		//TODO: find the way to do this properly
		for (auto& track : tracks) {
			if (track.first != currentChord && track.first != Chord::Drum) {
				track.second->read(temp.get(), bufferSize);
			}
		}

		return samplesRead;
	}

	void Instrument::setChord(Chord c)
	{
		currentChord = c;
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

	float Instrument::getProportionalPosition()
	{
		if (tracks.size() < 1) {
			return 0;
		}

		auto tr = tracks.begin()->second;
		return (float)tr->getPosition() / tr->getLength();
	}

	size_t Instrument::getPosition()
	{
		//if map is empty return 0
		if (tracks.size() < 1)
			return 0;
		//else return position of the first track
		//tracks must be equal length and are played all at once so
		//they should always have the same position
		return tracks.begin()->second->getPosition();
	}

	Instrument::~Instrument() {
	}
}