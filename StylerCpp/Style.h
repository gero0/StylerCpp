#pragma once
#include <unordered_map>
#include <string>
#include "Part.h"

namespace Styler {
	struct Style {
		std::string name;
		int tempo;
		int metrum;
		std::unordered_map<std::string, Part> parts;
		std::vector<std::string> instruments;
	};
}
