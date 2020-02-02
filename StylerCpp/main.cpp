#include <portaudio.h>
#include <iostream>
#include "player.h"
#include <sndfile.h>
#include "windows.h"

int main() {
	Styler::Player player;

	player.initialize();
	player.play();

	Sleep(10000);
}