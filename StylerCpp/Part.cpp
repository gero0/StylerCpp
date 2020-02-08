#include "Part.h"
#include "Exceptions.h"

namespace Styler {
	Part::Part(size_t bufferSize) {
		instrumentBuffer = new float[bufferSize];
	}
	
	Part::Part(Part&& other) noexcept{
		instrumentBuffer = other.instrumentBuffer;
		//moving map
		instruments.merge(other.instruments);
		other.instrumentBuffer = nullptr;
	}

	//TODO: Add some checks later if necessary
	void Part::addInstrument(std::string name, Instrument instrument) {
		instruments.insert({ name, std::move(instrument) });
	}

	void Part::changeVolume(std::string instrument, float volume)
	{
		auto iterator = instruments.find(instrument);

		if (iterator != instruments.end()) {
			iterator->second.setVolume(volume);
		}
	}

	void Part::setChord(Chord chord)
	{
		for (auto& ins : instruments) {
			ins.second.setChord(chord);
		}
	}

	void Part::setPosition(size_t position)
	{
		for (auto& ins : instruments) {
			ins.second.setPosition(position);
		}
	}

	void Part::setProportionalPosition(float position)
	{
		for (auto& ins : instruments) {
			ins.second.setProportionalPosition(position);
		}
	}

	std::vector<std::string> Part::getInstrumentNames()
	{
		std::vector<std::string> names;

		for (auto& ins : instruments) {
			names.push_back(ins.first);
		}

		return names;
	}

	/*
		Reads mixed samples and writes them to specified buffer, offset - offset of buffer, count - max number of samples to read
		It will not exceed number of samples in track.
		Returns number of read samples.
	*/
	size_t Part::readStream(float* buffer, size_t offset, size_t count) {
		std::fill(buffer, buffer + count - 1, 0); //fill buffer with zeros
		size_t samplesRead = 0;
		for (auto& ins : instruments) {
			samplesRead = ins.second.read(instrumentBuffer, count);
			for (int i = 0; i < samplesRead; i++) {
				buffer[i] += instrumentBuffer[i];
			}
		}
		return samplesRead;
	}

	Part::~Part() {
		delete[] instrumentBuffer;
	}
}