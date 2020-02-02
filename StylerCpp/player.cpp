#include "windows.h"

#include "player.h"
#include "PartManager.h"
#include "Exceptions.h"
#include <stdlib.h>
#include <cstdint>

namespace Styler 
{
	const size_t bufferSize = 32;

	Player::Player() : pManager(bufferSize){
		stream = nullptr;
		Pa_Initialize();
	}

	bool Player::initialize() {
		auto error = Pa_OpenDefaultStream(&stream
			, 0                     /* no input */
			, 2         /* stereo out */
			, paFloat32             /* floating point */
			, 44100
			, bufferSize/2
			, portAudioCallback
			, &pManager);        /* our sndfile data struct */

		if (error != paNoError) {
			return false;
		}

		return true;
	}

	void Player::play() {
		Pa_StartStream(stream);
		//temporary;
		Sleep(1500);
		pManager.ins.setChord(Chord::F);
		Sleep(1500);
		pManager.ins.setChord(Chord::C);
	}

	void Player::stop() {
		Pa_StopStream(stream);
	}

	Player::~Player() {
		Pa_Terminate();
	}
}



