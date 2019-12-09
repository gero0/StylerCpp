#include "PartManager.h"
#include "Exceptions.h"
#include <sndfile.h>
#include <portaudio.h>
#include "player.h"
#include <iostream>

namespace Styler {
	PartManager::PartManager() {
		
	}

	size_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		if (!currentPart)
		{
			throw new NullPointerException;
		}
		return currentPart->readStream(buffer, offset, count);
	}
}