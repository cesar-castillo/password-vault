
#ifndef GOD_H_
#define GOD_H_

#include <gtkmm.h>
#include "pvdbmanager/db_manager.hpp"

class God
{
public:
	static Glib::RefPtr<Gtk::Builder> get_builder()
	{
		if (!builder_)
		{
			try
			{
				builder_ = Gtk::Builder::create_from_file("../gtkmm_glade_ui.glade");
			}
			catch(const Glib::FileError& ex)
			{
				std::cout << "Error: " << ex.what() << std::endl;
			}
		}

		return builder_;
	}

	static Gtk::Window* get_window()
	{
		if (!window_) builder_->get_widget("main_window", window_);
		return window_;
	}

	static std::shared_ptr<core::DbManager> get_db_manager()
	{
		static auto db_manager_ = std::make_shared<core::DbManager>();
		return db_manager_;
	}

	static std::string get_db_filename()
	{
		return current_db_filename_;
	}

	static void set_db_filename(const std::string& db_filename)
	{
		current_db_filename_ = db_filename;
	}

	static void destroy_objects()
	{
		delete window_;
	}

private:
	// Declaration (not definition) of static data members
	static Glib::RefPtr<Gtk::Builder> builder_;
	static Gtk::Window* window_;
	static std::string current_db_filename_;
};

#endif