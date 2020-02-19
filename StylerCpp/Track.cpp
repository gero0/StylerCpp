#include "Track.h"
#include <sndfile.h>
namespace Styler {

	Track::Track(std::string filepath, size_t channels, size_t sampleRate) {
		fileInfo.format = 0;
		file = sf_open(filepath.c_str(), SFM_READ, &fileInfo);
		if (sf_error(file) != SF_ERR_NO_ERROR) {
			throw new InvalidAudioFileException;
		}
		auto e = sf_error_number(sf_error(file));
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
		//TODO: Check for errors (check libsndfile documentation)

		this->position = position;
		sf_seek(file, position, SEEK_SET);
	}

	void Track::setProportionalPosition(float position)
	{
		if (position < 0 || position > 1)
			throw new ArgumentException;

		setPosition((double)position * length);
	}
	
	size_t Track::getLength() const
	{
		return length;
	}

	size_t Track::read(float* buffer, size_t count)
	{
		auto samplesRead = sf_read_float(file, buffer, (sf_count_t)count);
		position += samplesRead;
		return samplesRead;
	}

	Track::~Track(){
		if(file != nullptr) 
			sf_close(file);
	}
}
