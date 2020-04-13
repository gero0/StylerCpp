#include "Rectangle.h"

namespace Styler {
	Rectangle::Rectangle()
	{
		Rectangle(1, 1);
	}

	Rectangle::Rectangle(int x, int y) : Gtk::Widget()
	{	
		width = x;
		height = y;
		set_has_window(true);
	}

	Rectangle::~Rectangle()
	{
	}

	bool Rectangle::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
	{
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		auto refStyleContext = get_style_context();

		int x = allocation.get_x();
		int y = allocation.get_y();

		//cr->rectangle(x, y, width, height);
		cr->set_source_rgb(255, 0, 0);
		cr->fill();
		cr->paint();
		cr->stroke();

		return true;
	}

	void Rectangle::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const
	{
		minimum_width = width;
		natural_width = width;
	}

	void Rectangle::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const
	{
		minimum_height = height;
		natural_height = height;
	}

	void Rectangle::get_preferred_height_for_width_vfunc(int /*width*/, int& minimum_height, int& natural_height) const
	{
		minimum_height = height;
		natural_height = height;
	}

	void Rectangle::get_preferred_width_for_height_vfunc(int /*height*/, int& minimum_width, int& natural_width) const
	{
		minimum_width = width;
		natural_width = width;
	}

	void Rectangle::on_size_allocate(Gtk::Allocation& allocation)
	{
		if (m_refGdkWindow)
		{
			m_refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(),
				allocation.get_width(), allocation.get_height());
		}
	}

	void Rectangle::on_realize()
	{
		set_realized();
		if (!m_refGdkWindow)
		{
			//Create the GdkWindow:

			GdkWindowAttr attributes;
			memset(&attributes, 0, sizeof(attributes));

			Gtk::Allocation allocation = get_allocation();

			//Set initial position and size of the Gdk::Window:
			attributes.x = allocation.get_x();
			attributes.y = allocation.get_y();
			attributes.width = allocation.get_width();
			attributes.height = allocation.get_height();

			attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
			attributes.window_type = GDK_WINDOW_CHILD;
			attributes.wclass = GDK_INPUT_OUTPUT;

			m_refGdkWindow = Gdk::Window::create(get_parent_window(), &attributes,
				GDK_WA_X | GDK_WA_Y);
			set_window(m_refGdkWindow);

			//make the widget receive expose events
			m_refGdkWindow->set_user_data(gobj());
		}
	}


}
