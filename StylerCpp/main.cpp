//disable macros that conflict with gtk
#define NOMINMAX

//TODO: Remember to enable optimisation for release!

#include <portaudio.h>
#include <iostream>

#include "windows.h"
#include "MainWindow.h"
#include <gtkmm.h>
#include "MidiCom.h"

int main(int argc, char** argv) {
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Styler::Player player;
	player.initializeStream();

	midi_com::midi_player_ptr = &player;
	midi_com::enumerateDevices();
	midi_com::startRecording(0);

	Styler::MainWindow window(&player);
	window.set_default_size(800, 600);

	auto res = app->run(window);
	std::cout << res;
}