#pragma once
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <Windows.h>
#include "MidiDevice.h"
#include "Player.h"

namespace midi_com
{
	extern Styler::Player* midi_player_ptr;
	void chordInterpretLoop();

	void enumerateDevices();
	bool openDeviceRead(UINT id);
	bool closeDeviceRead(UINT id);
	bool openDeviceWrite(UINT id);
	bool closeDeviceWrite(UINT id);
	bool startRecording(UINT id);
	bool stopRecording(UINT id);

	void writeToDevice(UINT id, DWORD msg);

	void __stdcall MidiCallback(
		HMIDIIN   hMidiIn,
		UINT      wMsg,
		DWORD_PTR dwInstance,
		DWORD_PTR dwParam1,
		DWORD_PTR dwParam2
	);

	void handleMidiData(DWORD_PTR dwParam1, DWORD_PTR dwParam2);
} // namespace MCom
