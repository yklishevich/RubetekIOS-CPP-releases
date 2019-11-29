
namespace svl
{
	user_widget::user_widget(color back_color)
	{
		w32_create_child_window(0, 0, win32::rgb_from_color(back_color));
	}

	user_widget::~user_widget()
	{
		start_destruction();
		w32_destroy_window();
	}
}