//disable macros that conflict with gtk
#define NOMINMAX

#include <portaudio.h>
#include <iostream>
#include "player.h"
#include <sndfile.h>
#include "windows.h"
#include "MainWindow.h"
#include <gtkmm.h>

int main(int argc, char** argv) {
	
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Styler::Player player;
	player.initialize();

	Styler::MainWindow window(&player);
	window.set_default_size(800, 600);

	auto res = app->run(window);
	std::cout << res;

	Sleep(20000);
}