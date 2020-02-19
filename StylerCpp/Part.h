#pragma once
#include "Instrument.h"
#include <string>
#include <unordered_map>
#include <cstdint>
#include <memory>

namespace Styler{
	class Part
	{
	public:
		Part() {};
		Part(size_t bufferSize);
		~Part();
		size_t readStream(float* buffer, size_t offset, size_t count);
		void addInstrument(std::string name, Instrument instrument);
		void changeVolume(std::string instrument, float volume);
		void setChord(Chord chord);
		void setPosition(size_t position);
		void setProportionalPosition(float position);
		std::vector<std::string> getInstrumentNames();
		PartType type = PartType::Main;
	private:
		std::shared_ptr<float[]> instrumentBuffer;
		std::unordered_map < std::string, Instrument > instruments;
	};
};


