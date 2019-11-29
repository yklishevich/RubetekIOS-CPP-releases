#ifndef SVL_BASE_DIALOGS_HPP
#define SVL_BASE_DIALOGS_HPP

#include <svl/base/environment.hpp>
#include <svl/base/form.hpp>

namespace svl
{
	struct desktop_t 
	{
		static unsigned displays_count();
		static ::svl::rect display_rect(unsigned display_no = 0);
		static ::svl::rect pri_display_rect();
		static ::svl::rect pri_display_work_rect();
		static ::svl::rect rect();
	};
	const desktop_t desktop = desktop_t();

	namespace dlg_option { enum
	{
		overwrite_prompt = 0x01
	};}

	struct selected_file
	{
		string   file_name;
		unsigned filter_index;
		operator safe_bool() const;
	};

	struct selected_files
	{
		std::vector<string> file_names;
		unsigned            filter_index;
		operator safe_bool() const;
	};
	
	namespace buttons
	{
		enum _
		{
			yes,
			no,
			cancel
		};
	}

	/*----------------------------------------------------------------------*/

	void show_info(form&,     str_ref text, str_ref caption = app_name());
	void show_info(desktop_t, str_ref text, str_ref caption = app_name());

	void show_warning(form&,     str_ref text, str_ref caption = app_name());
	void show_warning(desktop_t, str_ref text, str_ref caption = app_name());

	void show_error(form&,     str_ref text, str_ref caption = app_name());
	void show_error(desktop_t, str_ref text, str_ref caption = app_name());

	buttons::_ ask_yes_no_question(form&,     str_ref text, str_ref caption = app_name());
	buttons::_ ask_yes_no_question(desktop_t, str_ref text, str_ref caption = app_name());
	buttons::_ ask_yes_no_cancel_question(form&,     str_ref text, str_ref caption = app_name());
	buttons::_ ask_yes_no_cancel_question(desktop_t, str_ref text, str_ref caption = app_name());

	string select_path(form&, str_ref initial_path = SVL_T(""), str_ref text = SVL_T(""));

	// filter: "Все файлы (*.*);;Документы (*.doc *.docx)"
	selected_file  get_open_file_name (form&, str_ref filter = SVL_T(""), unsigned filter_index = 0, str_ref initial_path = SVL_T(""), str_ref caption = SVL_T(""));
	selected_files get_open_file_names(form&, str_ref filter = SVL_T(""), unsigned filter_index = 0, str_ref initial_path = SVL_T(""), str_ref caption = SVL_T(""));
	selected_file  get_save_file_name (form&, str_ref filter = SVL_T(""), unsigned filter_index = 0, str_ref initial_path = SVL_T(""), str_ref caption = SVL_T(""), unsigned flags = dlg_option::overwrite_prompt);
}

#endif // SVL_BASE_DIALOGS_HPP