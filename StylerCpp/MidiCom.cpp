#include "MidiCom.h"
#include <thread>
#include <vector>
#include <map>
#include <mmeapi.h>

namespace midi_com
{
	using Clock = std::chrono::high_resolution_clock;

	std::unique_ptr<std::thread> chordInterpretingThread;
	std::mutex chordMutex;
	Styler::Player* midi_player_ptr;

	const BYTE MIDI_Note_On = 0x90;
	const BYTE MIDI_Control_Change = 0xB0;
	const BYTE MIDI_Start_Sequence = 0xFA;
	const BYTE MIDI_Stop_Sequence = 0xFC;
	const BYTE MIDI_Clock = 0xF8;
	const BYTE MIDI_Active_Sensing = 0xFE;

	bool recording = false;
	bool closing = false;
	HANDLE inputPipe;
	HANDLE outputPipe;
	UINT activeMidiChannel = 0;
	UINT activeDeviceId = 0;
	UINT midiDevicesCount = 0;
	std::vector<MidiDevice> midiDevices;
	std::vector<Styler::Note> receivedNotes;

	static std::map <std::vector<Styler::Note>, Styler::Chord> chordNotes{
			{ { Styler::Note::C, Styler::Note::E, Styler::Note::G }, Styler::Chord::C },

			{ { Styler::Note::D, Styler::Note::FS, Styler::Note::A }, Styler::Chord::D },

			{ { Styler::Note::E, Styler::Note::GS, Styler::Note::B }, Styler::Chord::E },

			{ { Styler::Note::F, Styler::Note::A, Styler::Note::C }, Styler::Chord::F },

			{ { Styler::Note::G, Styler::Note::B, Styler::Note::D }, Styler::Chord::G },

			{ { Styler::Note::A, Styler::Note::CS, Styler::Note::E }, Styler::Chord::A },

			{ { Styler::Note::B, Styler::Note::DS, Styler::Note::FS }, Styler::Chord::B },

			{ { Styler::Note::C, Styler::Note::DS, Styler::Note::G }, Styler::Chord::Cm },

			{ { Styler::Note::D, Styler::Note::F, Styler::Note::A }, Styler::Chord::Dm },

			{ { Styler::Note::E, Styler::Note::G, Styler::Note::B }, Styler::Chord::Em },

			{ { Styler::Note::F, Styler::Note::GS, Styler::Note::C }, Styler::Chord::Fm },

			{ { Styler::Note::G, Styler::Note::AS, Styler::Note::D }, Styler::Chord::Gm },

			{ { Styler::Note::A, Styler::Note::C, Styler::Note::E }, Styler::Chord::Am },

			{ { Styler::Note::B, Styler::Note::D, Styler::Note::FS}, Styler::Chord::Bm }
	};

	bool compareChords(std::vector<Styler::Note> first, std::vector<Styler::Note> second, int minimum_matches)
	{
		int matches = 0;

		for(auto note : first)
		{
			if (std::find(second.begin(), second.end(), note) != second.end())
				matches++;
		}

		return matches >= minimum_matches;
	}

	Styler::Chord detectChord(std::vector<Styler::Note> notes) {
		for(auto chord : chordNotes)
		{
			if (compareChords(notes, chord.first, chord.first.size()))
			{
				//std::cout << "CHORD DETECTED: " + chord.Value;
				return chord.second;
			}
		}

		return Styler::Chord::None;
	}

	void chordInterpretLoop(){
		auto startTime = Clock::now();

		while (recording)
		{
			while (std::chrono::duration_cast<std::chrono::milliseconds>( Clock::now() - startTime ).count() <= 15)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

			chordMutex.lock();
			if (receivedNotes.size() == 0)
			{
				startTime = Clock::now();
				chordMutex.unlock();
				continue;
			}

			Styler::Chord c = detectChord(receivedNotes);

			if (c != Styler::Chord::None)
				midi_player_ptr->pManager.setChord(c);

			receivedNotes.clear();

			chordMutex.unlock();
		}
	}

	void enumerateDevices()
	{
		midiDevicesCount = midiInGetNumDevs();
		auto midiOutputDevicesCount = midiOutGetNumDevs();

		for (UINT i = 0; i < midiDevicesCount; i++)
		{
			MIDIINCAPS caps;
			const auto cap_ptr = reinterpret_cast<LPMIDIINCAPSA>(&caps);
			midiInGetDevCapsA(i, cap_ptr, sizeof(MIDIINCAPS));
			MidiDevice dev{ static_cast<unsigned int>(i), caps };
			midiDevices.push_back(dev);
			activeDeviceId = 0;
		}
	}

	bool openDeviceRead(UINT id)
	{
		if (midiDevices.size() <= id)
			return false;

		bool result;
		try
		{
			result = midiDevices[id].openRead();
		}
		catch (std::exception& e)
		{
			std::cerr << "Could not open device!: " << e.what();
			return false;
		}

		return result;
	}

	bool closeDeviceRead(UINT id) {
		if (midiDevices.size() <= id)
			return false;

		bool result;
		try
		{
			result = midiDevices[id].closeRead();
		}
		catch (std::exception& e)
		{
			std::cerr << "Could not open device!: " << e.what();
			return false;
		}

		return true;
	}

	bool openDeviceWrite(UINT id) {
		if (midiDevices.size() <= id)
			return false;

		bool result;
		try
		{
			result = midiDevices[id].openWrite();
		}
		catch (std::exception& e)
		{
			std::cerr << "Could not open device!: " << e.what() ;
			return false;
		}

		return result;
	}

	bool closeDeviceWrite(UINT id) {


		if (midiDevices.size() <= id)
			return false;

		bool result;
		try
		{
			result = midiDevices[id].closeWrite();
		}
		catch (std::exception& e)
		{
			std::cerr << "Could not open device!: " << e.what();
			return false;
		}

		return true;
	}

	bool startRecording(UINT id)
	{
		if (midiDevices.size() <= id)
			return false;

		openDeviceRead(id);

		recording = true;
		std::cout << "Recording " << id << " started\n";

		chordInterpretingThread = std::make_unique<std::thread>(chordInterpretLoop);

		return midiDevices[id].startRecording();
	}

	bool stopRecording(UINT id)
	{
		recording = false;
		chordInterpretingThread->join();

		if (midiDevices.size() <= id)
			return false;

		closeDeviceRead(id);
		return midiDevices[id].stopRecording();
	}

	void writeToDevice(UINT id, DWORD msg) {
		if (midiDevices.size() <= id)
			return;
		std::cout << "MIDI OUT: " << std::hex << (int)msg << "\n";
		midiDevices[id].writeMsg(msg);
	}

	void __stdcall MidiCallback(
		HMIDIIN   hMidiIn,
		UINT      wMsg,
		DWORD_PTR dwInstance,
		DWORD_PTR dwParam1,
		DWORD_PTR dwParam2)
	{

		if (dwParam1 != MIDI_Active_Sensing && dwParam1 != MIDI_Clock) { //ignore Active Sensing message
			std::cout << "\nMidi callback: "
				<< std::hex << wMsg << " "
				<< dwParam1;
		}


		if (wMsg == MIM_DATA && dwParam1 != MIDI_Active_Sensing)
		{
			handleMidiData(dwParam1, dwParam2);
		}
	}

	void handleMidiData(DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		if (dwParam1 == MIDI_Start_Sequence) {
			midi_player_ptr->play();
			return;
		}
		else if (dwParam1 == MIDI_Stop_Sequence) {
			midi_player_ptr->stop();
			return;
		}

		auto hiword = HIWORD(dwParam1);

		if (hiword == 0)
			return;

		auto data = LOWORD(dwParam1);
		BYTE status = *(BYTE*)(&dwParam1); //extract the status byte...

		if (status - MIDI_Control_Change == activeMidiChannel) {
			BYTE vol = *(((BYTE*)(&dwParam1)) + 2);
			midi_player_ptr->pManager.masterVolume = ( (float)vol / 255);
		}

		if (status - MIDI_Note_On == activeMidiChannel) {
			//This ugly piece of code extracts the second byte we need - the note byte
			BYTE note = *(((BYTE*)(&dwParam1)) + 1);

			while (note > 35) note -= 12;

			chordMutex.lock();
			receivedNotes.push_back(static_cast<Styler::Note>(note));
			chordMutex.unlock();
		}
	}

	void getDevices() {
		std::string message{ "" };

		for (auto d : midiDevices) {
			message += (char*)d.capabilities.szPname;
			message += ";";
		}

		char buffer[2048];
		strcpy_s(buffer, message.c_str());

		DWORD nbr;

		BOOL result = WriteFile(
			outputPipe,
			buffer,
			strlen(buffer) + 1,
			&nbr,
			NULL
		);
	}

} // namespace midi_com

