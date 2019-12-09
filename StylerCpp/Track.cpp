#include "Track.h"

namespace Styler {
	Track::Track(std::string filepath, size_t channels, size_t sampleRate) {
		file = sf_open(filepath.c_str(), SFM_READ, &fileInfo);
		if (channels != fileInfo.channels || sampleRate != fileInfo.samplerate) {
			throw new InvalidAudioFileException;
		}
		length = fileInfo.frames;
	}

	size_t Track::getPosition() const
	{
		return position;
	}

	void Track::setPosition(size_t position)
	{
		this->position = position;
		sf_seek(file, position, SEEK_SET);
	}
	
	size_t Track::getLength() const
	{
		return length;
	}

	size_t Track::read(float* buffer, size_t count)
	{
		auto samplesRead = sf_read_float(file, buffer, count);
		position += samplesRead;
		return samplesRead;
	}

	Track::~Track(){
		sf_close(file);
	}
}
