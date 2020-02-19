#pragma once
#include <unordered_map>
#include <string>
#include "Part.h"

namespace Styler {
	struct Style {
		std::string name;
		int tempo = 120;
		int metrum = 4;
		std::unordered_map<std::string, Part> parts;
		std::vector<std::string> instruments;
	};
}
