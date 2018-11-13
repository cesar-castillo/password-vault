
#ifndef APP_CONTROLLER_H_
#define APP_CONTROLLER_H_

#include <gtkmm.h>
#include "pvdbmanager/database.hpp"
#include "pvdbmanager/db_manager.hpp"

#include "password_page.hpp"
#include "opendb_page.hpp"

class AppController
{
public:
	AppController(int argc, char* argv[]);
	~AppController();
	void run();

private:
	void on_new_db_clicked();
	void on_open_db_clicked();
	void on_save_db_clicked();
	void on_close_db_clicked();
	void on_add_pass_clicked();

	void clear_password_list();

private:
	std::shared_ptr<core::DbManager> db_manager_;
	std::unique_ptr<PasswordPage> password_page_;
	std::unique_ptr<OpendbPage> opendb_page_;

	// Gtk widgets members
	Gtk::Window* window_ = nullptr;
	Gtk::TreeView* treeview1_ = nullptr;
	Gtk::Stack* stack1_ = nullptr;
	Gtk::ToolButton* new_db_btn_ = nullptr;
	Gtk::ToolButton* open_db_btn_ = nullptr;
	Gtk::ToolButton* save_db_btn_ = nullptr;
	Gtk::ToolButton* close_db_btn_ = nullptr;
	Gtk::ToolButton* add_pass_btn_ = nullptr;

	// Other Gtk objects
	Glib::RefPtr<Gtk::ListStore> liststore1_;
	Glib::RefPtr<Gtk::Application> gtk_app_;
	Glib::RefPtr<Gtk::Builder> builder_;
};

#endif