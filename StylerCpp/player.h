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
	static int _stdcall portAudioCallback (
		const void* inputBuffer,
		void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData)
	{
		float* output = (float*)outputBuffer;

		memset(output, 0, sizeof(float) * framesPerBuffer * 2);
		
		//zast¹piæ userData
		//pManager.readStream(output, 0, framesPerBuffer * 2);
	
		return paContinue;
	}

	class Player {
		//The number of samples read by last call of the portAudioCallback
		int samplesRead = 0;	
		//Pointer to trackManager	
		PartManager pManager;
	};
}

