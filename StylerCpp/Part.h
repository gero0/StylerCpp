#pragma once
#include "Instrument.h"
#include <string>
#include <unordered_map>
#include <cstdint>

namespace Styler{
	enum class PartType {
		Main,
		Intro,
		Ending,
		Fill,
		Break
	};

	class Part
	{
	public:
		Part(size_t bufferSize);
		Part(const Part&) = delete;
		Part(Part&& other) noexcept;
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
		float* instrumentBuffer;
		std::unordered_map < std::string, Instrument > instruments;
	};
};


