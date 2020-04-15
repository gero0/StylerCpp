#pragma once
#include "Instrument.h"
#include <string>
#include <unordered_map>
#include <cstdint>
#include <memory>

namespace Styler {
	class Part
	{
	public:
		Part() {};
		Part(size_t bufferSize);
		~Part();

		PartType type = PartType::Main;
		std::string fillTrack = "";
		int length = 1;
		int styleInstrumentCount = 1;

		void addInstrument(std::string name, Instrument instrument);
		void changeVolume(std::string instrument, float volume);
		void setChord(Chord chord);
		void setPosition(size_t position);
		void setProportionalPosition(float position);

		float getProportionalPosition();
		size_t readStream(float* buffer, size_t offset, size_t count);
		std::vector<std::string> getInstrumentNames();
		
	private:
		std::shared_ptr<float[]> instrumentBuffer;
		std::unordered_map < std::string, Instrument > instruments;
	};
};
