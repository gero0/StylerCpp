#pragma once
#include "portaudio.h"
#include "PartManager.h"
#include "Style.h"
#include "Metronome.h"

namespace Styler {

	enum class PlayerState {
		Playing,
		Stopped,
		NotLoaded
	};

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
		void setPart(std::string trackName);
		std::vector<std::string> getPartNames();
		std::vector<std::string> getInstrumentNames();
		PartManager pManager;
		PlayerState state = PlayerState::NotLoaded;
		Metronome metronome;
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

			switch (manager.currentPart->second.type) {
				case PartType::Main:
					manager.currentPart->second.setPosition(0);
					break;
				case PartType::Ending:
					player->state = PlayerState::Stopped;
					manager.setPosition(0);
					manager.setChord(Chord::Drum);
					player->metronome.stop();
					return paComplete;
					break;
				default:
					manager.setPart(manager.nextPart);
					break;
			}
		}
	
		return paContinue;
	}

	
}

