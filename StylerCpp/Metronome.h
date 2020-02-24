#pragma once

#include <thread>

namespace Styler {
    class Metronome
    {
    public:
        int metrum = 4;
        int bars = 2;
        int tempo = 1;
        void start();
        void stop();
        int getBeat();
        int getBar();
        bool isRunning();
    private:
        int barCounter = 1;
        int beatCounter = 1;
        bool looping = false;;
        std::thread* thread;
        float timePerBeat;
        void loop();
    };
}



