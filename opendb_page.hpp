

#ifndef OPENDB_PAGE_H_
#define OPENDB_PAGE_H_

#include <gtkmm.h>
#include "pvdbmanager/database.hpp"
#include "pvdbmanager/db_manager.hpp"

class OpendbPage
{
public:
	OpendbPage(const std::string& invoker_page_name);
	
	~OpendbPage();
	void show();
	void set_db_filename(const std::string& db_filename);
	
private:
	void initialize();
	void go_to_page(std::string page_name);
	void clear_text_entries();

	void on_cancel_button_clicked();
	void on_ok_button_clicked();

private:
	Gtk::Entry* opendb_password_entry_ = nullptr;
	Gtk::Entry* opendb_keyfile_entry_ = nullptr;
	Gtk::Button* opendb_cancel_btn_ = nullptr;
	Gtk::Button* opendb_ok_btn_ = nullptr;
	Gtk::Stack* stack1_;

	Glib::RefPtr<Gtk::Builder> builder_;
	std::shared_ptr<core::DbManager> db_manager_;
	std::string invoker_page_name_;

	std::string db_filename_;
};

#endif