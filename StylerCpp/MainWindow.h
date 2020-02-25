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
		const int standard_button_width = 64;
		const int standard_button_height = 64;

		Player* player;

		std::unordered_map<int, Chord> chordMap{
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

		Gtk::Grid grid;
		Gtk::Button loadButton;
		Gtk::Button unloadButton;
		Gtk::Button playStopButton;

		void playStop();
		void loadStyle();
		void addSliders();
		void addPartButtons();
		void sliderHandler(Glib::RefPtr<Gtk::Adjustment> adjustment, std::string instrument);
		void masterVolumeSliderHandler(Glib::RefPtr<Gtk::Adjustment> adjustment);
		void partButtonHandler(std::string part);
	};
}
