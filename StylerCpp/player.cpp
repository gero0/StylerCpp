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
		loader.loadFromJson(std::filesystem::path { "C:/Users/kacpe/Desktop/Output/style.json" } );
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
		
		Part part(bufferSize);

		Instrument ins(bufferSize);
		ins.addTrack(Chord::Drum, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 01 - Audio - Drums.wav");
		Instrument ins2(bufferSize);
		ins2.addTrack(Chord::C, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 02 - Audio - Bas C.wav");
		ins2.addTrack(Chord::F, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 03 - Audio - Bas F.wav");
		Instrument ins3(bufferSize);
		ins3.addTrack(Chord::C, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 05 - Audio - Gitara C.wav");
		ins3.addTrack(Chord::F, "C:\\Users\\kacpe\\Desktop\\Output\\main_a\\main_a - 06 - Audio - Gitara F.wav");

		part.addInstrument("Drums", std::move(ins));
		part.addInstrument("Bas", std::move(ins2));
		part.addInstrument("Gitara", std::move(ins3));

		pManager.addPart("main_a", std::move(part));
		pManager.setPart("main_a");
		pManager.setChord(Chord::C);
		
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

	Player::~Player() {
		Pa_Terminate();
	}
}



