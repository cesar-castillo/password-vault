
#ifndef PASSWORD_SCREEN_H_
#define PASSWORD_SCREEN_H_

#include <gtkmm.h>
#include "pvdbmanager/database.hpp"
#include "pvdbmanager/db_manager.hpp"

class PasswordPage
{
public:
	PasswordPage(const std::string& invoker_page_name);
	
	~PasswordPage();
	void show();
	
private:
	void initialize();
	void uninitialize();
	void go_to_page(std::string page_name);
	void clear_text_entries();

	void on_cancel_button_clicked();
	void on_save_button_clicked();

private:
	Gtk::Entry* title_entry_ = nullptr;
	Gtk::Entry* username_entry_ = nullptr;
	Gtk::Entry* password_entry_ = nullptr;
	Gtk::Entry* password2_entry_ = nullptr;
	Gtk::Entry* url_entry_ = nullptr;
	Gtk::Entry*	description_entry_ = nullptr;
	Gtk::Button* cancel_button_ = nullptr;
	Gtk::Button* save_button_ = nullptr;
	Gtk::Stack* stack1_;

	Glib::RefPtr<Gtk::Builder> builder_;
	std::shared_ptr<core::DbManager> db_manager_;
	std::string invoker_page_name_;
};

#endif