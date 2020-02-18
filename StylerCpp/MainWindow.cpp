#include "MainWindow.h"
#include "Loader.h"
#include <iostream>

namespace Styler {

	MainWindow::MainWindow(Player* playerPtr) {
		player = playerPtr;
		set_title("Styler");

		chordMap =
		{
			{ GDK_KEY_a, Chord::C },
			{ GDK_KEY_w, Chord::Cm },
			{ GDK_KEY_s, Chord::D },
			{ GDK_KEY_e, Chord::Dm },
			{ GDK_KEY_d, Chord::E },
			{ GDK_KEY_f, Chord::F },
			{ GDK_KEY_t, Chord::Fm },
			{ GDK_KEY_g, Chord::G },
			{ GDK_KEY_y, Chord::Gm },
			{ GDK_KEY_h, Chord::A },
			{ GDK_KEY_u, Chord::Am },
			{ GDK_KEY_j, Chord::B }
		};

		this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::onKeyPress), false);
	}

	bool MainWindow::onKeyPress(GdkEventKey* event)
	{
		auto isChord = setChord(event->keyval);

		if (isChord)
			return true;

		player->playPause();

		return false;
	}

	bool MainWindow::setChord(int key)
	{
		auto result = chordMap.find(key);

		if(result == chordMap.end())
			return false;

		player->pManager.setChord(result->second);
		return true;
	}
}