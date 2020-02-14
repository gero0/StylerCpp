#include <iostream>

#include "PartManager.h"
#include "Exceptions.h"
#include "Instrument.h"
#include "player.h"

namespace Styler {
	PartManager::PartManager(size_t bufferSize) {
		bSize = bufferSize;
		currentPart = nullptr;
	}

	size_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		if (!currentPart)
		{
			throw new NullPointerException;
		}

		accessLock.lock();

		auto samplesRead = currentPart->readStream(buffer, offset, count);
		if (samplesRead < count) {
			if (currentPart->type == PartType::Main) {
				currentPart->setPosition(0);
			}
		}

		accessLock.unlock();

		return samplesRead;
	}

	void PartManager::addPart(std::string partName, Part part) {
		parts.insert({ partName, std::move(part) });
	}

	void PartManager::setPart(std::string partName)
	{
		accessLock.lock();

		auto iter = parts.find(partName);

		if (iter != parts.end()) {
			currentPart = &(iter->second);
		}

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

		currentChord = chord;

		accessLock.lock();

		for (auto& part : parts) {
			part.second.setChord(chord);
		}

		accessLock.unlock();
	}

	void PartManager::setPosition(size_t position)
	{
		accessLock.lock();

		if (currentPart)
			currentPart->setPosition(position);

		accessLock.unlock();
	}

	void PartManager::setProportionalPosition(float position)
	{
		accessLock.lock();

		if (currentPart)
			currentPart->setProportionalPosition(position);

		accessLock.unlock();
	}

	std::vector<std::string> PartManager::getInstrumentNames()
	{
		if(!currentPart)
			return std::vector<std::string>();

		return currentPart->getInstrumentNames();
	}
}