#include "PartManager.h"

namespace Styler {
	uint32_t PartManager::readStream(float* buffer, size_t offset, size_t count) {
		if (!currentTrack)
		{
			//throw error
		}
		return currentTrack->readStream(buffer, offset, count);
	}
}