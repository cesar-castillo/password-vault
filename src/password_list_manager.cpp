
#include "password_list_manager.hpp"
#include "god.hpp"

PasswordListManager::PasswordListManager()
{
	builder_ = God::get_builder();
	db_manager_ = God::get_db_manager();
}

PasswordListManager::~PasswordListManager()
{

}

PasswordListManager& PasswordListManager::get_instance()
{
	static PasswordListManager instance;
	return instance;
}

void PasswordListManager::fill_password_list()
{
	auto liststore1 = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(builder_->get_object("liststore1"));
	auto all_passwords = db_manager_->get_all_entries();

	if (liststore1)
	{
		clear_password_list();
		
		for (auto password : all_passwords)
		{
			auto row = *(liststore1->append());

			row.set_value(0, std::string(password.title));
			row.set_value(1, std::string(password.username));
			row.set_value(2, std::string(password.website));
			row.set_value(3, std::string(password.description));
			row.set_value(4, Gdk::Pixbuf::create_from_file("../resources/icons/065.png"));
		}
	}
}

void PasswordListManager::clear_password_list()
{
	auto liststore1 = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(builder_->get_object("liststore1"));
	if (liststore1) liststore1->clear();
}