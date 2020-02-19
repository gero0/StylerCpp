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
	const size_t bufferSize = 64;

	Player::Player() : pManager(bufferSize){
		stream = nullptr;
		Pa_Initialize();
		
		//temporary
		Loader loader(bufferSize);

		style = loader.loadFromJson(std::filesystem::path { "C:/Users/kacpe/Desktop/Output/style.json" } );
		pManager.moveParts(style.parts);
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
		playing = true;
	}

	void Player::stop() {
		Pa_StopStream(stream);
		playing = false;
	}

	void Player::playPause()
	{
		if (playing)
			stop();   
		else
			play();
	}

	void Player::loadStyle(Style style)
	{
		/*this->style = style;
		pManager.moveParts(style.parts);*/
	}

	Player::~Player() {
		Pa_Terminate();
	}
}



