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
		playStopButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::playStop));
		loadButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::loadStyle));

		Gtk::Image* image = Gtk::manage<Gtk::Image>(new Gtk::Image("icons/PlayPause.png"));
		image->show();

		loadButton.add_pixlabel("info.xpm", "Load Style");
		unloadButton.add_pixlabel("info.xpm", "Unload Style");
		//playStopButton.add_pixlabel("info.xpm", "Play/Stop");
		playStopButton.set_image(*image);
		playStopButton.set_always_show_image(true);

		add(grid);

		//grid.set_row_homogeneous(true);
		grid.set_column_homogeneous(true);
		grid.attach(loadButton, 0, 0, 1, 1);

		grid.attach(loadButton, 0, 0, 1, 1);
		grid.attach(unloadButton, 1, 0, 1, 1);
		grid.attach(playStopButton, 2, 0, 1, 1);

		loadButton.show();
		unloadButton.show();
		playStopButton.show();
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

	void MainWindow::partButtonHandler(std::string part) {
		player->setPart(part);
	}

	void MainWindow::addSliders()
	{
		//We added 3 buttons. We want sliders to appear in collumns on the right of buttons
		//In case more buttons are added change this value
		int collumn = 3;

		for (const auto& instrument : player->getInstrumentNames()) {	
			
			Gtk::Label* label = Gtk::manage<Gtk::Label>(new Gtk::Label());
			label->set_text(instrument);
			label->show();

			Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(50, 0.0, 101.0, 0.1, 1.0, 1.0);

			Gtk::Scale* slider = Gtk::manage<Gtk::Scale>(new Gtk::Scale(adjustment, Gtk::Orientation::ORIENTATION_VERTICAL));
			slider->set_inverted(true);
			slider->show();
			slider->set_size_request(20, 600);

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
		for (const auto& part : player->getPartNames()) {
			Gtk::Button* button = Gtk::manage<Gtk::Button>(new Gtk::Button);
			button->add_label(part);
			button->show();
			button->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &MainWindow::partButtonHandler), part));
			grid.add(*button);
		}
	}
}