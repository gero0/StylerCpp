#include "PartManager.h"
#include "Exceptions.h"
#include "Instrument.h"
#include <sndfile.h>
#include <portaudio.h>
#include "player.h"
#include <iostream>

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
		auto samplesRead = currentPart->readStream(buffer, offset, count);
		if (samplesRead < count) {
			if (currentPart->type == PartType::Main) {
				currentPart->setPosition(0);
			}
		}

		return samplesRead;
	}

	void PartManager::addPart(std::string partName, Part part) {
		parts.insert({ partName, std::move(part) });
	}

	void PartManager::setPart(std::string partName)
	{
		auto iter = parts.find(partName);

		if (iter != parts.end()) {
			currentPart = &(iter->second);
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
		for (auto& part : parts) {
			part.second.setChord(chord);
		}
	}

	void PartManager::setPosition(size_t position)
	{
		if (currentPart)
			currentPart->setPosition(position);
	}

	void PartManager::setProportionalPosition(float position)
	{
		if (currentPart)
			currentPart->setProportionalPosition(position);
	}

	std::vector<std::string> PartManager::getInstrumentNames()
	{
		if(!currentPart)
			return std::vector<std::string>();

		return currentPart->getInstrumentNames();
	}
}