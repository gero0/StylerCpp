#include "MainWindow.h"
#include "StyleLoader.h"
#include "Rectangle.h"

#include <iostream>
#include <algorithm>

namespace Styler {
	MainWindow::MainWindow(Player* playerPtr)
	{
		player = playerPtr;
		
		set_title("Styler");
		this->signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::onKeyPress), false);


		//LOAD Button
		loadButton.add_label("Load Style");
		loadButton.set_size_request(standard_button_width, standard_button_height);
		loadButton.show();
		loadButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::loadStyle));

		//UNLOAD Button
		unloadButton.add_label("Unload Style");
		unloadButton.set_size_request(standard_button_width, standard_button_height);
		unloadButton.show();
		//TODO: signal handler

		//PLAY/STOP Button
		Gtk::Image* image = Gtk::manage<Gtk::Image>(new Gtk::Image("icons/PlayPause.png"));
		image->set_size_request(standard_button_width, standard_button_height);
		image->show();

		playStopButton.set_image(*image);
		playStopButton.set_size_request(standard_button_width, standard_button_height);
		playStopButton.show();
		playStopButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::playStop));

		//MASTER VOLUME Slider
		Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(50, 0.0, 101.0, 0.1, 1.0, 1.0);
		Gtk::Scale* slider = Gtk::manage<Gtk::Scale>(new Gtk::Scale(adjustment, Gtk::Orientation::ORIENTATION_HORIZONTAL));
		slider->set_size_request(300, standard_button_height);
		slider->show();
		adjustment->signal_value_changed().connect(
			sigc::bind<Glib::RefPtr<Gtk::Adjustment>>(sigc::mem_fun(*this, &MainWindow::masterVolumeSliderHandler), adjustment));

		//AUDIO APIS and DEVICES Combo Boxes
		setApiComboBox();
		setDeviceComboBox();

		//Setting up MAIN BOX and MAIN GRID
		show_all_children();
		add(mainBox);
		//mainGrid.set_row_homogeneous(true);
		//mainGrid.set_column_homogeneous(true);
		mainGrid.attach(loadButton, 0, 0, 1, 1);
		mainGrid.attach(unloadButton, 1, 0, 1, 1);
		mainGrid.attach(playStopButton, 2, 0, 1, 1);
		mainGrid.attach(*slider, 3, 0, 1, 1);

		mainGrid.show();

		mainBox.add(mainGrid);
		mainBox.show();
	}

	bool MainWindow::onKeyPress(GdkEventKey* event)
	{
		auto isChord = setChord(event->keyval);

		if (isChord)
			return true;

		player->playStop();

		return false;
	}

	//Handles changing chord by keyboard input
	bool MainWindow::setChord(int key)
	{
		auto result = chordMap.find(key);

		if (result == chordMap.end())
			return false;

		player->pManager.setChord(result->second);
		return true;
	}

	void MainWindow::playStop()
	{
		player->playStop();
	}

	//Opens the file explorer to select style file
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
		drawLeds();
	}

	//TODO: dew it
	void MainWindow::drawLeds() {
	}

	void MainWindow::onDeviceChanged()
	{
		auto index = audioDevices.get_active_row_number();
		player->setAudioDevice(index);
	}

	void MainWindow::setApiComboBox()
	{
		for (auto& api : player->getAudioApis()) {
			audioApis.append(api->name);
		}
		audioApis.set_active(0);
		audioApis.show();
		mainGrid.attach(audioApis, 0, 1, 3, 1);
		audioApis.signal_changed().connect(sigc::mem_fun(*this,
			&MainWindow::onApiChanged));
	}

	void MainWindow::setDeviceComboBox()
	{
		for (auto& device : player->getOutputDevices()) {
			audioDevices.append(device->name);
		}
		audioDevices.set_active(0);
		audioDevices.show();
		mainGrid.attach(audioDevices, 0, 2, 3, 1);
		audioDevices.signal_changed().connect(sigc::mem_fun(*this,
			&MainWindow::onDeviceChanged));
	}

	void MainWindow::onApiChanged() {
		auto index = audioApis.get_active_row_number();
		player->setHostApi(index);
		
		audioDevices.remove_all();
		setDeviceComboBox();
	}

	void MainWindow::addSliders()
	{
		int collumn = 0;

		for (const auto& instrument : player->getInstrumentNames()) {
			Gtk::Label* label = Gtk::manage<Gtk::Label>(new Gtk::Label());
			label->set_size_request(standard_button_width, standard_button_height);
			label->set_text(instrument);
			label->show();

			Glib::RefPtr<Gtk::Adjustment> adjustment = Gtk::Adjustment::create(50, 0.0, 101.0, 0.1, 1.0, 1.0);

			Gtk::Scale* slider = Gtk::manage<Gtk::Scale>(new Gtk::Scale(adjustment, Gtk::Orientation::ORIENTATION_VERTICAL));
			slider->set_inverted(true);
			slider->show();
			slider->set_size_request(80, 400);

			//bind pointer and intrument to adjustment signal
			adjustment->signal_value_changed().connect(
				sigc::bind< Glib::RefPtr<Gtk::Adjustment>, std::string >
				(sigc::mem_fun(*this, &MainWindow::sliderHandler), adjustment, instrument)
			);

			sliderGrid.attach(*slider, collumn, 2, 1, 1);
			sliderGrid.attach(*label, collumn, 1, 1, 1);
			collumn++;
		}
		sliderGrid.show();
		mainBox.add(sliderGrid);
	}

	void MainWindow::addPartButtons()
	{
		int collumn = 0;

		auto parts = player->getPartNames(false);
		partCount = parts.size();

		buttonGrid.attach(ledGrid, 0, 0, partCount, 1);

		for (const auto& part : parts) {
			Gtk::Button* button = Gtk::manage<Gtk::Button>(new Gtk::Button);
			button->add_label(part);
			button->show();
			button->set_size_request(standard_button_width, standard_button_height * (4.0 / 5));
			button->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &MainWindow::partButtonHandler), part));
			buttonGrid.attach(*button, collumn, 1, 1, 1);
			collumn++;
		}

		buttonGrid.show();
		mainBox.add(buttonGrid);
	}
}