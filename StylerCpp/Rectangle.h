#pragma once
#include <gtkmm.h>

namespace Styler {
	class Rectangle : public Gtk::Widget
	{
	public:
		Rectangle();
		Rectangle(int x, int y);
		~Rectangle();
	protected:
		bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
		void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
		void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
		void get_preferred_height_for_width_vfunc(int /*width*/, int& minimum_height, int& natural_height) const;
		void get_preferred_width_for_height_vfunc(int /*height*/, int& minimum_height, int& natural_height) const;
		void on_size_allocate(Gtk::Allocation& allocation);
		void on_realize();

		Glib::RefPtr<Gdk::Window> m_refGdkWindow;
	private:
		int width = 1;
		int height = 1;
	};
}
