#include "windows.h"

#include "player.h"
#include "PartManager.h"
#include "Exceptions.h"
#include "Loader.h"
#include <stdlib.h>
#include <cstdint>
#include <filesystem>

#include <iostream>

namespace Styler 
{
	const int channelCount = 2; //stereo
	const int sampleRate = 44100;
	const size_t bufferSize = 64;

	Player::Player() : pManager(bufferSize){
		Pa_Initialize();
		stream = nullptr;

		auto devCount = Pa_GetDeviceCount();

		int listIndex = 0;

		for (int actualIndex = 0; actualIndex < devCount; actualIndex++) {
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(actualIndex);
			
			if (deviceInfo->maxOutputChannels >= channelCount) {
				outputDevices.push_back(deviceInfo);
				outputDevicesIndexMap.insert({ listIndex, actualIndex } );
				listIndex++;
			}
		}

		//Set first output device as active device
		activeDeviceId = outputDevicesIndexMap.begin()->second;
	}

	bool Player::initialize() {
		/*
		auto error = Pa_OpenDefaultStream(&stream
			, 0                     //no input
			, channelCount			// stereo out
			, paFloat32             // floating point 
			, sampleRate
			, bufferSize/channelCount
			, portAudioCallback
			, this);			// we give the callback function pointer to player so it can control it
			
			*/

		PaStreamParameters params;

		params.channelCount = channelCount;
		params.device = activeDeviceId;
		params.hostApiSpecificStreamInfo = NULL;
		params.sampleFormat = paFloat32;
		params.suggestedLatency = Pa_GetDeviceInfo(activeDeviceId)->defaultLowOutputLatency;

		auto error = Pa_OpenStream(&stream, NULL, &params, sampleRate, bufferSize/channelCount, paNoFlag, portAudioCallback, this);

		if (error != paNoError) {
			std::cout << "ERROR: " << Pa_GetErrorText(error);
			return false;
		}

		return true;
	}

	void Player::play() {
		if (state != PlayerState::Stopped)
			return;

		//Need to call StopStream in case stream was stopped by file reaching its end
		Pa_StopStream(stream);
		Pa_StartStream(stream);
		state = PlayerState::Playing;
		metronome.start();
	}

	void Player::stop() {
		Pa_StopStream(stream);
		pManager.setPosition(0);
		pManager.setChord(Chord::Drum);
		state = PlayerState::Stopped;
		metronome.stop();
	}

	void Player::playStop()
	{
		if (state == PlayerState::Playing)
			stop();   
		else
			play();
	}

	void Player::loadFromJson(std::string filePath)
	{
		Loader loader(bufferSize);
		style = loader.loadFromJson({ filePath });
		pManager.addParts(style.parts);
		state = PlayerState::Stopped;

		metronome.metrum = style.metrum;
		metronome.tempo = style.tempo;
		metronome.bars = pManager.currentPart->second.length;
	}

	void Player::setPart(std::string trackName)
	{
		if (metronome.isRunning()) {
			int lastBeat = metronome.getBeat();
			while (metronome.getBeat() == lastBeat)
			{
				//wait
			}
		}

		
		pManager.changePart(trackName, state == PlayerState::Playing, &metronome, style.metrum);
	}

	void Player::changeAudioDevice(size_t index)
	{
		if (state == PlayerState::Playing)
			stop();

		activeDeviceId = outputDevicesIndexMap[index];

		initialize();

	}

	std::vector<std::string> Player::getInstrumentNames()
	{
		return style.instruments;
	}
	
	std::vector<std::string> Player::getPartNames(bool full) {
		std::vector<std::string> names;
		for (auto part : style.parts) {
			if (part.second.type == PartType::Fill && !full)
				continue;
			names.push_back(part.first);
		}
		return names;
	}

	Player::~Player() {
		Pa_Terminate();
	}
}



