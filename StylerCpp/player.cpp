#include <windows.h>
#include <stdlib.h>
#include <cstdint>
#include <filesystem>
#include <iostream>

#include "player.h"
#include "PartManager.h"
#include "Exceptions.h"
#include "StyleLoader.h"

namespace Styler
{
	const int channelCount = 2;
	const int sampleRate = 44100;
	const size_t bufferSize = 64;

	Player::Player() : pManager(bufferSize) {
		Pa_Initialize();
		
		enumerateApis();
		enumerateDevices();	

		//Set first output device as active device
		activeDeviceId = outputDevicesIndexMap.begin()->second;
	}

	void Player::enumerateApis() {
		hostApis.clear();
		auto apiCount = Pa_GetHostApiCount();

		for (int i = 0; i < apiCount; i++) {
			const PaHostApiInfo* apiInfo = Pa_GetHostApiInfo(i);
			hostApis.push_back(apiInfo);
		}
	}

	void Player::enumerateDevices() 
	{
		outputDevices.clear();
		outputDevicesIndexMap.clear();
		//Enumerate all audio devices of selected host API and check what output devices are available
		auto deviceCount = hostApis[activeHostApiId]->deviceCount;

		int listIndex = 0;

		//outputDevicesIndexMap is used to map the index of the output device in the outputDevices vector to device's id
		for (int actualIndex = 0; actualIndex < deviceCount; actualIndex++) {
			const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(actualIndex);

			if (deviceInfo->maxOutputChannels >= channelCount) {
				outputDevices.push_back(deviceInfo);
				outputDevicesIndexMap.insert({ listIndex, actualIndex });
				listIndex++;
			}
		}
	}

	bool Player::initializeStream() {
		PaStreamParameters params;

		params.channelCount = channelCount;
		params.device = Pa_HostApiDeviceIndexToDeviceIndex(activeHostApiId, activeDeviceId);
		params.hostApiSpecificStreamInfo = NULL;
		params.sampleFormat = paFloat32;
		params.suggestedLatency = Pa_GetDeviceInfo(activeDeviceId)->defaultLowOutputLatency;

		auto error = Pa_OpenStream(&stream, NULL, &params, sampleRate, bufferSize / channelCount, paNoFlag, portAudioCallback, this);

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
		StyleLoader loader(bufferSize);

		try {
			style = loader.loadFromJson({ filePath });
		}
		catch (std::exception & e) {
			std::cerr << "An error occured opening the style file: " << e.what();
			return;
		}

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
				//wait for the next beat before changing track
			}
		}

		pManager.changePart(trackName, state == PlayerState::Playing, &metronome, style.metrum);
	}

	void Player::setAudioDevice(size_t index)
	{
		if (state == PlayerState::Playing)
			stop();

		activeDeviceId = outputDevicesIndexMap[index];

		initializeStream();
	}

	void Player::setHostApi(size_t index)
	{
		if (state == PlayerState::Playing)
			stop();

		activeHostApiId = index;
		enumerateDevices();
		activeDeviceId = outputDevicesIndexMap.begin()->second;

		initializeStream();
	}

	const std::vector<std::string> Player::getInstrumentNames() const
	{
		return style.instruments;
	}

	const std::vector<std::string> Player::getPartNames(bool includeFill) const {
		std::vector<std::string> names;
		for (auto part : style.parts) {
			if (part.second.type == PartType::Fill && !includeFill)
				continue;
			names.push_back(part.first);
		}
		return names;
	}

	Player::~Player() {
		Pa_Terminate();
	}
}