#include "Part.h"
#include "Exceptions.h"

namespace Styler {
	Part::Part(size_t bufferSize) {
		instrumentBuffer = new float[bufferSize];
	}

	/*
		Reads mixed samples and writes them to specified buffer, offset - offset of buffer, count - max number of samples to read
		It will not exceed number of samples in track.
		Returns number of read samples.
	*/
	size_t Part::readStream(float* buffer, size_t offset, size_t count) {
		/*size_t lastPos = offset;
		buffer += offset;
		while (position < length && position - lastPos < count) {
			*buffer = getSample(position);
			buffer++;
		}

		if (looped && position == length - 1)
			position = 0;

		return (position - lastPos);*/
		return 0;
	}

	Part::~Part() {
		delete[] instrumentBuffer;
	}
}