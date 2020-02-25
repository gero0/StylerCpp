#include "MainWindow.h"
#include "Loader.h"

#include <iostream>
#include <algorithm>

namespace Styler {

	MainWindow::MainWindow(Player* playerPtr)
	{
		player = playerPtr;
		set_title("Styler");

		this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::onKeyPress), false);


		loadButton.add_pixlabel("info.xpm", "Load Style");
		loadButton.set_size_request(standard_button_width, standard_button_height);
		loadButton.show();
		loadButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::loadStyle));

		unloadButton.add_pixlabel("info.xpm", "Unload Style");
		unloadButton.set_size_request(standard_button_width, standard_button_height);
		unloadButton.show();

		Gtk::Image* image = Gtk::manage<Gtk::Image>(new Gtk::Image("icons/PlayPause.png"));
		image->set_size_request(standard_button_width, standard_button_height);
		image->show();
	
		playStopButton.set_image(*image);
		playStopButton.set_always_show_image(true);
		playStopButton.set_size_request(standard_button_width, standard_button_height);
		playStopButton.show();
		playStopButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::playStop));

		//Creating master volume slider
		Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(50, 0.0, 101.0, 0.1, 1.0, 1.0);
		Gtk::Scale* slider = Gtk::manage<Gtk::Scale>(new Gtk::Scale(adjustment, Gtk::Orientation::ORIENTATION_HORIZONTAL));
		slider->set_size_request(400, standard_button_height);
		slider->show();
		adjustment->signal_value_changed().connect(
			sigc::bind<Glib::RefPtr<Gtk::Adjustment>>(sigc::mem_fun(*this, &MainWindow::masterVolumeSliderHandler), adjustment));

		add(grid);

		//grid.set_row_homogeneous(true);
		//grid.set_column_homogeneous(true);
		grid.attach(loadButton, 0, 0, 1, 1);
		grid.attach(loadButton, 0, 0, 1, 1);
		grid.attach(unloadButton, 1, 0, 1, 1);
		grid.attach(playStopButton, 2, 0, 1, 1);
		grid.attach(*slider, 3, 0, 1, 1);
		
		grid.show();	
	}

	bool MainWindow::onKeyPress(GdkEventKey* event)
	{
		auto isChord = setChord(event->keyval);

		if (isChord)
			return true;

		player->playStop();

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
	void MainWindow::playStop()
	{
		player->playStop();
	}

	void MainWindow::loadStyle()
	{
		Gtk::FileChooserDialog dialog("Choose a Style JSON file", Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.set_transient_for(*this);
		dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
		dialog.add_button("Select", Gtk::RESPONSE_OK);

		int result = dialog.run();

		switch (result) {
		case Gtk::RESPONSE_OK:
			std::string path = dialog.get_filename();
			std::replace(path.begin(), path.end(), '\\', '/');
			player->loadFromJson(path);
			addSliders();
			addPartButtons();
			break;
		}
	}

	void MainWindow::sliderHandler(Glib::RefPtr<Gtk::Adjustment> adjustment, std::string instrument) {
		player->pManager.setVolume(instrument, adjustment->get_value() / 100);
	}

	void MainWindow::masterVolumeSliderHandler(Glib::RefPtr<Gtk::Adjustment> adjustment)
	{
		player->pManager.masterVolume = adjustment->get_value() / 100;
	}

	void MainWindow::partButtonHandler(std::string part) {
		player->setPart(part);
	}

	void MainWindow::addSliders()
	{
		//We added 3 buttons. We want sliders to appear in collumns on the right of buttons
		//In case more buttons are added change this value
		int collumn = 4;

		for (const auto& instrument : player->getInstrumentNames()) {	
			
			Gtk::Label* label = Gtk::manage<Gtk::Label>(new Gtk::Label());
			label->set_text(instrument);
			label->show();

			Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(50, 0.0, 101.0, 0.1, 1.0, 1.0);

			Gtk::Scale* slider = Gtk::manage<Gtk::Scale>(new Gtk::Scale(adjustment, Gtk::Orientation::ORIENTATION_VERTICAL));
			slider->set_inverted(true);
			slider->show();
			slider->set_size_request(80, 600);

			//bind pointer and intrument to adjustment signal
			adjustment->signal_value_changed().connect(
				sigc::bind< Glib::RefPtr<Gtk::Adjustment>, std::string >
				(sigc::mem_fun(*this, &MainWindow::sliderHandler), adjustment, instrument )
			);
		
			grid.attach(*slider, collumn, 1,1,1);
			grid.attach(*label, collumn, 0,1,1);
			collumn++;
		}
	}
	
	void MainWindow::addPartButtons()
	{
		for (const auto& part : player->getPartNames(false)) {
			Gtk::Button* button = Gtk::manage<Gtk::Button>(new Gtk::Button);
			button->add_label(part);
			button->show();
			button->set_size_request(standard_button_width, standard_button_height);
			button->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &MainWindow::partButtonHandler), part));
			grid.add(*button);
		}
	}
}