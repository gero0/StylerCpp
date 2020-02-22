#pragma once
#include "portaudio.h"
#include "PartManager.h"
#include "Style.h"

namespace Styler {

	class Player {
	public:
		Player();
		~Player();
		//The number of samples read by last call of the portAudioCallback
		int samplesRead = 0;
		bool initialize();
		void play();
		void stop();
		void playStop();
		void loadFromJson(std::string filePath);
		std::vector<std::string> getInstrumentNames();
		//replace with state later?
		bool playing = false;
		PartManager pManager;
	private:
		PaStream* stream;
		Style style;
	};


	/*
	The PortAudio callback function.
	It reads samples from the main buffer,
	and plays the sound
	*/
	static int portAudioCallback (
		const void* inputBuffer,
		void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData)
	{
		float* output = (float*)outputBuffer;

		size_t count = (double)framesPerBuffer * 2;

		memset(output, 0, sizeof(float) * framesPerBuffer * 2);
		
		Player* player = (Player*)userData;
		PartManager& manager = player->pManager;

		auto framesRead = manager.readStream(output, 0, count);

		if (framesRead < count) {
			if (manager.currentPart->second.type == PartType::Main) {
				manager.currentPart->second.setPosition(0);
			}
			if (manager.currentPart->second.type == PartType::Ending) {
				player->playing = false;
				manager.setPosition(0);
				manager.setChord(Chord::Drum);
				return paComplete;
			}
		}
	
		return paContinue;
	}

	
}

