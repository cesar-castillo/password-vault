
#include "app_controller.hpp"
#include "password_list_manager.hpp"
#include "god.hpp"
#include <exception>

AppController::AppController(int argc, char* argv[])
{
	gtk_app_ = Gtk::Application::create(argc, argv);
	builder_ = God::get_builder();

	//db_manager_ = std::make_shared<core::DbManager>();
	db_manager_ = God::get_db_manager();
	password_page_ = std::make_unique<PasswordPage>("password_list_page");
	opendb_page_ = std::make_unique<OpendbPage>("password_list_page");

	window_ = God::get_window();

	// Throw error if no main window
	if (!window_) throw std::runtime_error("Main window could not be located!");

	window_->set_title("Password Vault");

	// Getting all main screen widgets
	builder_->get_widget("stack1", stack1_);
	builder_->get_widget("treeview1", treeview1_);
	builder_->get_widget("new_db", new_db_btn_);
	builder_->get_widget("open_db", open_db_btn_);
	builder_->get_widget("save_db", save_db_btn_);
	builder_->get_widget("close_db", close_db_btn_);
	builder_->get_widget("add_pass", add_pass_btn_);
	liststore1_ = Glib::RefPtr<Gtk::ListStore>::cast_dynamic(builder_->get_object("liststore1"));

	// Connect signals on corresponding widgets
	if (new_db_btn_)
		new_db_btn_->signal_clicked().connect(sigc::mem_fun(
			*this, &AppController::on_new_db_clicked));

	if (open_db_btn_)
		open_db_btn_->signal_clicked().connect(sigc::mem_fun(
			*this, &AppController::on_open_db_clicked));

	if (save_db_btn_)
		save_db_btn_->signal_clicked().connect(sigc::mem_fun(
			*this, &AppController::on_save_db_clicked));

	if (close_db_btn_)
		close_db_btn_->signal_clicked().connect(sigc::mem_fun(
			*this, &AppController::on_close_db_clicked));

	if (add_pass_btn_)
		add_pass_btn_->signal_clicked().connect(sigc::mem_fun(
			*this, &AppController::on_add_pass_clicked));
}

AppController::~AppController()
{
	// Release pointers
	delete window_;
	delete new_db_btn_;
	delete open_db_btn_;
	delete save_db_btn_;
	delete close_db_btn_;
	delete add_pass_btn_;
	delete treeview1_;
	delete stack1_;
}

void AppController::run()
{
	// Run the app
	gtk_app_->run(*window_);
}

void AppController::on_new_db_clicked()
{
	Gtk::FileChooserDialog dialog("Please name the database file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*window_);
	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch (result)
	{
		case Gtk::RESPONSE_OK:
		{
			std::string db_file_name = dialog.get_filename();

			if (db_file_name.length() > 0)
			{
				db_manager_->create_database(db_file_name, "123456");
				std::cout << "Database created!" << std::endl;
			}

			break;
		}
		default:
		{
			std::cout << "Unknown selection" << std::endl;
			break;
		}
	}
}

void AppController::on_open_db_clicked()
{
	Gtk::FileChooserDialog dialog("Please choose a database file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*window_);

	//Add response buttons the the dialog:
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:
	auto filter_dat = Gtk::FileFilter::create();
	filter_dat->set_name("Databases");
	filter_dat->add_pattern("*.dat");
	dialog.add_filter(filter_dat);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	//Handle the response:
	switch (result)
	{
		case Gtk::RESPONSE_OK:
		{
		  	std::string filename = dialog.get_filename();
		  	opendb_page_->set_db_filename(filename);
		  	opendb_page_->show();
		  	
		  	break;
		}
		default:
		{
		  	std::cout << "Unexpected button clicked." << std::endl;
		  	break;
		}
	}
}

void AppController::on_save_db_clicked()
{
	if (!db_manager_->save_database("123456"))
		std::cout << "There was an error while saving the database!" << std::endl;
	else
	{
		std::cout << "Database saved!" << std::endl;
		//Change window title to reflect db changes
		std::string window_title = window_->get_title();
		window_title.erase(window_title.end() - 1);
		window_->set_title(window_title);
		// Disable Save toolbar button
  		save_db_btn_->set_sensitive(false);
	}
}

void AppController::on_close_db_clicked()
{
	db_manager_->close_database();
	clear_password_list();
	window_->set_title("Password Vault");
	
	// Disable Save and Add Password toolbar buttons
	save_db_btn_->set_sensitive(false);
	add_pass_btn_->set_sensitive(false);
	close_db_btn_->set_sensitive(false);

	std::cout << "Database closed!" << std::endl;
}

void AppController::on_add_pass_clicked()
{
	password_page_->show();
}

void AppController::clear_password_list()
{
	liststore1_->clear();
}