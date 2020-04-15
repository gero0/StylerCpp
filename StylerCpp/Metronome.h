#pragma once

#include <thread>

namespace Styler {
	class Metronome
	{
	public:
		int barCounter = 1;
		int beatCounter = 1;
		int metrum = 4;
		int bars = 2;
		int tempo = 1;

		void start();
		void stop();
		void reset();

		bool isRunning();
		int getBeat();
		int getBar();

	private:
		bool looping = false;
		bool restart = false;
		std::thread* thread;
		float timePerBeat;

		void loop();
	};
}
