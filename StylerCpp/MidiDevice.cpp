#include "MidiDevice.h"
#include "MidiCom.h"
#include <iostream>

namespace midi_com
{
	MidiDevice::MidiDevice(unsigned int deviceId, MIDIINCAPS deviceCapabilities)
	{
		id = deviceId;
		capabilities = deviceCapabilities;
	}

	bool MidiDevice::isReadOpened() const
	{
		return openedRead;
	}

	bool MidiDevice::isWriteOpened() const {
		return openedWrite;
	}

	bool MidiDevice::startRecording()
	{
		if (!openedRead)
			return false;

		const auto buffer_result = midiInAddBuffer(
			deviceInputHandle, &midiInputHeader, sizeof(MIDIHDR)
		);

		if (buffer_result != MMSYSERR_NOERROR)
		{
			return false;
		}

		const auto start_result = midiInStart(deviceInputHandle);

		if (start_result != MMSYSERR_NOERROR)
		{
			return false;
		}

		return true;
	}

	bool MidiDevice::stopRecording() const
	{
		const auto stop_result = midiInStop(deviceInputHandle);
		return stop_result == MMSYSERR_NOERROR;
	}

	bool MidiDevice::openWrite() {
		const MMRESULT openResult = midiOutOpen(
			&deviceOutputHandle,
			id,
			NULL,
			NULL,
			CALLBACK_NULL
		);

		if (openResult != MMSYSERR_NOERROR)
		{
			openedWrite = false;
			return false;
		}

		midiOutputHeader.lpData = output_buffer;
		midiOutputHeader.dwBufferLength = buffer_length_output;
		midiOutputHeader.dwFlags = 0;
		const auto headerResult = midiOutPrepareHeader(deviceOutputHandle, &midiOutputHeader, sizeof(midiOutputHeader));

		if (headerResult != MMSYSERR_NOERROR) {
			openedWrite = false;
			return false;
		}

		openedWrite = true;
		return true;
	}

	bool MidiDevice::closeWrite() {
		openedWrite = false;
		midiOutUnprepareHeader(deviceOutputHandle, &midiOutputHeader, sizeof(midiOutputHeader));

		midiOutReset(deviceOutputHandle);

		auto result = midiOutClose(
			deviceOutputHandle
		);

		if (result == MMSYSERR_NOERROR)
			std::cout << "CLOSED WRITE";

		return result;
	}

	bool MidiDevice::openRead()
	{
		const DWORD msg = 0;
		const MMRESULT openResult = midiInOpen(
			&deviceInputHandle,
			id,
			reinterpret_cast<DWORD_PTR>(MidiCallback),
			msg,
			CALLBACK_FUNCTION
		);

		if (openResult != MMSYSERR_NOERROR)
		{
			openedRead = false;
			return false;
		}

		midiInputHeader.lpData = input_buffer;
		midiInputHeader.dwBufferLength = buffer_length_input;
		midiInputHeader.dwFlags = 0;

		const auto headerResult = midiInPrepareHeader(deviceInputHandle, &midiInputHeader, sizeof(MIDIHDR));

		if (headerResult != MMSYSERR_NOERROR)
		{
			openedRead = false;
			return false;
		}

		openedRead = true;
		return true;
	}

	bool MidiDevice::closeRead()
	{
		openedRead = false;
		midiInUnprepareHeader(deviceInputHandle, &midiInputHeader, sizeof(MIDIHDR));

		midiInReset(deviceInputHandle);

		auto result = midiInClose(
			deviceInputHandle
		);

		if (result == MMSYSERR_NOERROR)
			std::cout << "CLOSED READ";

		return result;
	}

	bool MidiDevice::writeMsg(DWORD msg) {
		if (!openedWrite)
			return false;

		auto result = midiOutShortMsg(deviceOutputHandle, msg);

		if (result != MMSYSERR_NOERROR) {
			std::cout << "AAA";
		}

		return result;
	}

	MidiDevice::~MidiDevice() {
		if (openedRead)
			closeRead();
		if (openedWrite)
			closeWrite();
	}
}