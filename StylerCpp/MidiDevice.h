#pragma once
#include <Windows.h>
#include <mmeapi.h>

namespace midi_com
{
	class MidiDevice
	{
	public:
		MidiDevice(unsigned int deviceId, MIDIINCAPS deviceCapabilities);
		unsigned int id;
		MIDIINCAPS capabilities{};
		bool openWrite();
		bool closeWrite();
		bool openRead();
		bool closeRead();
		bool startRecording();
		bool stopRecording() const;
		bool isReadOpened() const;
		bool isWriteOpened() const;
		bool writeMsg(DWORD msg);
		~MidiDevice();
	private:
		static constexpr int buffer_length_input = 32;
		static constexpr int buffer_length_output = 32;
		bool openedWrite = false;
		bool openedRead = false;
		HMIDIIN deviceInputHandle;
		HMIDIOUT deviceOutputHandle;
		MIDIHDR	midiInputHeader;
		MIDIHDR midiOutputHeader;
		char input_buffer[buffer_length_input];
		char output_buffer[buffer_length_output];
	};
}
