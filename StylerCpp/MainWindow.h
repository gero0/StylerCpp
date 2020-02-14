#pragma once
#include <gtkmm.h>
#include <unordered_map>
#include "player.h"

namespace Styler {
	class MainWindow : public Gtk::Window
	{
	public:
		MainWindow(Player*);
		bool onKeyPress(GdkEventKey*);
		bool setChord(int key);
	private:
		Player* player;
		std::unordered_map<int, Chord> chordMap;
	};
}



