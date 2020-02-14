#pragma once
#include "portaudio.h"
#include <sndfile.h>
#include "PartManager.h"

namespace Styler {
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

		memset(output, 0, sizeof(float) * framesPerBuffer * 2);
		
		PartManager* manager = (PartManager*)userData;

		manager->readStream(output, 0, (double)framesPerBuffer * 2);
	
		return paContinue;
	}

	class Player {
	public:
		//The number of samples read by last call of the portAudioCallback
		int samplesRead = 0;
		//Pointer to trackManager	
		Player();
		~Player();
		bool initialize();
		void play();
		void stop();
		void playPause();
		//replace with state later?
		bool playing = false;
		PartManager pManager;
	private:
		PaStream* stream;
	};
}

