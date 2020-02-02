#include "PartManager.h"
#include "Exceptions.h"
#include "Instrument.h"
#include <sndfile.h>
#include <portaudio.h>
#include "player.h"
#include <iostream>

namespace Styler {
	PartManager::PartManager(size_t bufferSize) : ins(bufferSize){
		bSize = bufferSize;
		currentPart = nullptr;

		//temporary
		ins.addTrack(Chord::C, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 02 - Audio - Bas C.wav");
		ins.addTrack(Chord::F, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 03 - Audio - Bas F.wav");
	}

	size_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		/*if (!currentPart)
		{
			throw new NullPointerException;
		}
		return currentPart->readStream(buffer, offset, count);*/

		size_t samplesRead = ins.read(buffer, count);

		return samplesRead;
	}
}