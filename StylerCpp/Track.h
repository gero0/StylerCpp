#include <sndfile.h>
#include <string>
#include "Enums.h"

namespace Styler {
	class Track {
	public:
		Track(std::string filepath, size_t channels = 2, size_t sampleRate = 44100);
		~Track();
		size_t getPosition() const;
		void setPosition(size_t position);
		size_t getLength() const;
		size_t read(float* buffer, size_t count);
	private:
		size_t position = 0;
		size_t length = 0;
		std::string filepath;
		SF_INFO fileInfo;
		SNDFILE* file;
	};
}
