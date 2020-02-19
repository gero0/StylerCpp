#pragma once
#include "Style.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <unordered_map>

namespace Styler {
	class Loader
	{
	public:
		Loader(size_t bufferSize);
		Style loadFromJson(std::filesystem::path filePath);
	private:
		size_t bufferSize;

		std::unordered_map<std::string, Chord> chordMap =
		{
			{ "Drums", Chord::Drum },
			{ "C", Chord::C },
			{ "D", Chord::D },
			{ "E", Chord::E },
			{ "F", Chord::F },
			{ "G", Chord::G },
			{ "A", Chord::A },
			{ "B", Chord::B },
			{ "Cm", Chord::Cm },
			{ "Dm", Chord::Dm },
			{ "Em", Chord::Em },
			{ "Fm", Chord::Fm },
			{ "Gm", Chord::Gm },
			{ "Am", Chord::Am },
			{ "Bm", Chord::Bm },
		};
	};
}
