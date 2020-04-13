#include <iostream>

#include "PartManager.h"
#include "Exceptions.h"
#include "Instrument.h"
#include "player.h"

namespace Styler {
	PartManager::PartManager(size_t bufferSize) {
		this->bufferSize = bufferSize;
		currentPart = parts.begin();
	}

	size_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		accessLock.lock();

		auto samplesRead = currentPart->second.readStream(buffer, offset, count);

		for (int i = 0; i < samplesRead; i++) {
			buffer[i] *= masterVolume;
		}

		accessLock.unlock();

		return samplesRead;
	}

	void PartManager::addParts(pMap partMap)
	{
		parts = partMap;
		//find first main track and set current part to it
		currentPart = std::find_if(parts.begin(), parts.end(),
			[](const std::pair<std::string, Part>& part) -> bool
			{
				return part.second.type == PartType::Main;
			}
		);
		setChord(Chord::Drum);
	}

	void PartManager::addPart(std::string partName, Part part) {
		parts.insert({ partName, part });
	}

	void PartManager::setPart(std::string partName, float position )
	{
		accessLock.lock();
		auto previousPart = currentPart;
		currentPart = parts.find(partName);
		currentPart->second.setProportionalPosition(position);
		previousPart->second.setPosition(0);
		accessLock.unlock();
	}

	void PartManager::changePart(std::string partName, bool isPlaying, Metronome* metro, int metrum)
	{
		//We don't need to do anything if we get request to change part to the same part
		if (currentPart->first == partName)
			return;

		//In case nothing is playing right now...
		if (!isPlaying) {
			if (currentPart->second.type == PartType::Intro) {
				//If currently intro is selected, clicking on another part will select it to be played after the intro
				if (parts[partName].type == PartType::Intro) {
					setPart(partName);
				}
				//If it's another intro part, just set it to play instead of current part
				else {
					nextPart = partName;
				}
			}
			//If intro is not currently selected...
			else {
				//set current part
				setPart(partName);
				//if new part is an intro, set the first main track as a default next part
				if (currentPart->second.type == PartType::Intro)
					nextPart = std::find_if(parts.begin(), parts.end(),
						[](const std::pair<std::string, Part>& part) -> bool
						{
							return part.second.type == PartType::Main;
						}
				)->first;
			}
		}
		
		//In case something is playing right now...
		//A lot of special cases here...
		else {
			//If the next part is an intro track...
			if (parts[partName].type == PartType::Intro) {
				//Set current part as the next part
				nextPart = currentPart->first;
				//Play intro part from the start and reset metronome to prevent desynchronisation
				setPart(partName, 0);
				metro->beatCounter = 1;
			}

			else if (currentPart->second.type == PartType::Intro) {
				nextPart = partName;
			}

			else if(currentPart->second.type == PartType::Main && parts[partName].type == PartType::Main){
				setPart(parts[partName].fillTrack, (float)(metro->getBeat() - 1) / metrum);
				nextPart = partName;
			}
			else if (currentPart->second.type == PartType::Fill || currentPart->second.type == PartType::Break) {
				nextPart = partName;
			}
			else if (parts[partName].type == PartType::Break) {
				nextPart = currentPart->first;
				setPart(partName);
			}
			else {
				setPart(partName, (float)(metro->getBeat()-1)/metrum);
			}
		}
	}

	void PartManager::setVolume(std::string instrument, float volume)
	{
		for (auto& part : parts) {
			part.second.changeVolume(instrument, volume);
		}
	}

	void PartManager::setChord(Chord chord)
	{
		if (currentChord == chord)
			return;

		accessLock.lock();
		currentChord = chord;

		for (auto& part : parts) {
			part.second.setChord(chord);
		}

		accessLock.unlock();
	}

	void PartManager::setPosition(size_t position)
	{
		accessLock.lock();

		currentPart->second.setPosition(position);

		accessLock.unlock();
	}

	void PartManager::setProportionalPosition(float position)
	{
		accessLock.lock();

		currentPart->second.setProportionalPosition(position);

		accessLock.unlock();
	}
}