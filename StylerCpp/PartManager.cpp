#include <iostream>

#include "PartManager.h"
#include "Exceptions.h"
#include "Instrument.h"
#include "player.h"

namespace Styler {
	PartManager::PartManager(size_t bufferSize) {
		bSize = bufferSize;
		currentPart = parts.begin();
	}

	size_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		accessLock.lock();

		auto samplesRead = currentPart->second.readStream(buffer, offset, count);
		
		accessLock.unlock();

		return samplesRead;
	}

	void PartManager::moveParts(std::unordered_map<std::string, Part> partMap)
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

	void PartManager::setPart(std::string partName)
	{
		accessLock.lock();

		currentPart = parts.find(partName);

		accessLock.unlock();
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