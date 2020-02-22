#include "windows.h"

#include "player.h"
#include "PartManager.h"
#include "Exceptions.h"
#include "Loader.h"
#include <stdlib.h>
#include <cstdint>
#include <filesystem>

namespace Styler 
{
	const int channelCount = 2; //stereo
	const int sampleRate = 44100;
	const size_t bufferSize = 64;

	Player::Player() : pManager(bufferSize){
		stream = nullptr;
		Pa_Initialize();
	}

	bool Player::initialize() {
		auto error = Pa_OpenDefaultStream(&stream
			, 0                     /* no input */
			, channelCount			/* stereo out */
			, paFloat32             /* floating point */
			, sampleRate
			, bufferSize/2
			, portAudioCallback
			, this);			/* our sndfile data struct */

		if (error != paNoError) {
			return false;
		}

		return true;
	}

	void Player::play() {
		//Need to call this in case stream was stopped before
		Pa_StopStream(stream);
		Pa_StartStream(stream);
		playing = true;
	}

	void Player::stop() {
		Pa_StopStream(stream);
		playing = false;
		pManager.setPosition(0);
		pManager.setChord(Chord::Drum);
	}

	void Player::playStop()
	{
		if (playing)
			stop();   
		else
			play();
	}

	void Player::loadFromJson(std::string filePath)
	{
		Loader loader(bufferSize);
		style = loader.loadFromJson({ filePath });
		pManager.moveParts(style.parts);
	}

	std::vector<std::string> Player::getInstrumentNames()
	{
		return style.instruments;
	}

	Player::~Player() {
		Pa_Terminate();
	}
}



