#include <fstream>
#include <iostream>
#include "Loader.h"
#include "Exceptions.h"
#include "Part.h"

namespace Styler {
	using json = nlohmann::json;
	namespace fs = std::filesystem;

	Loader::Loader(size_t bufferSize) {
		this->bufferSize = bufferSize;
	}

	Style Loader::loadFromJson(fs::path filePath) {

		//Opening file and checking if it exists

		std::ifstream file(filePath);
		if (!file)
			throw new ArgumentException;

		//Parsing JSON

		json j;
		file >> j;

		Style s;
		s.name = j["name"];
		s.tempo = j["tempo"];
		s.metrum = j["metrum"];

		for (auto& ins : j["instruments"]) {
			s.instruments.push_back(ins);
		}

		//Getting the directory of the JSON file
		//to search for tracks

		fs::path path(filePath);

		auto directory = path.remove_filename();
		
		for (auto& part : j["tracks"]) {
			Part p(bufferSize);
			std::string typeStr = part["Type"];
			p.type = typeMap[typeStr];

			//Part may not have filltrack and/or length defined.
			//In this case default values are set.
			//Since i don't know if there's a way to check if json object has
			//a specific field, i'm just gonna rely on exceptions in this case
			try {
				p.fillTrack = part["FillTrack"];
			}
			catch (const std::exception & e) {
				p.fillTrack = "";
			}
			
			try {
				p.length = part["Length"];
			}
			catch (const std::exception & e) {
				p.length = 1;
			}

			std::string name = part["Name"];
			
			
			//need to make a copy because append modifies value permanently
			auto dir = directory;
			auto partPath = dir.append(name);

			//Get paths to all files in the directory

			std::vector<std::string> files;

			for (auto f : fs::directory_iterator(partPath)) {
				files.push_back(f.path().string());
			}

			//it's time to construct objects
			for (auto& ins : s.instruments) {
				Instrument instrument(bufferSize);
				
				for (auto file : files) {
					
					//Find files with instrument name in their
					if (file.find(ins) != std::string::npos) {
						
						//Extract the chord name from the file name
						size_t firstindex = file.find_last_of(" ");
						size_t lastindex = file.find_last_of(".");
						
						std::string chordStr = file.substr(firstindex + 1, lastindex - firstindex - 1);
					
						Chord c = chordMap[chordStr];

						instrument.addTrack(c, file);
					}
				}
				p.addInstrument(ins, instrument);
			}

			s.parts.insert({ name, p });
		}
		return s;
	}	
}