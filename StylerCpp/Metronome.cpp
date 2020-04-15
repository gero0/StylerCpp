#include "Metronome.h"
#include <iostream>

namespace Styler {
	void Metronome::start()
	{
		timePerBeat = 1 / ((float)tempo / 60) * 1000;
		looping = true;
		thread = new std::thread(&Metronome::loop, this);
	}

	void Metronome::reset() {
		restart = true;
	}

	void Metronome::loop() {
		while (looping)
		{
			if (restart) {
				beatCounter = 1;
				barCounter = 1;
				restart = false;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds((int)timePerBeat));

			if (beatCounter >= metrum)
			{
				beatCounter = 1;
				if (barCounter >= bars)
					barCounter = 1;
				else
					barCounter++;
			}
			else
			{
				beatCounter++;
			}

			//std::cout << "Beat: " << beatCounter << " Bar:" << barCounter << "\n";
		}
	}

	int Metronome::getBeat()
	{
		return beatCounter;
	}

	int Metronome::getBar()
	{
		return barCounter;
	}

	void Metronome::stop()
	{
		looping = false;
		if (thread != nullptr) {
			thread->join();
			delete thread;
			thread = nullptr;
		}
		beatCounter = 1;
		barCounter = 1;
	}

	bool Metronome::isRunning()
	{
		return looping;
	}
}