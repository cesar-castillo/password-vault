
#include "opendb_page.hpp"
#include "password_list_manager.hpp"
#include "god.hpp"

OpendbPage::OpendbPage(const std::string& invoker_page_name)
{
	builder_ = God::get_builder();
	db_manager_ = God::get_db_manager();
	invoker_page_name_ = invoker_page_name;

	initialize();
}
	
OpendbPage::~OpendbPage()
{
	delete opendb_password_entry_;
	delete opendb_keyfile_entry_;
	delete opendb_cancel_btn_;
	delete opendb_ok_btn_ ;
}

void OpendbPage::show()
{
	go_to_page("opendb_page");
}

void OpendbPage::set_db_filename(const std::string& db_filename)
{
	db_filename_ = db_filename;
}
	
void OpendbPage::initialize()
{
	builder_->get_widget("stack1", stack1_);

	builder_->get_widget("opendb_password_entry", opendb_password_entry_);

	builder_->get_widget("opendb_keyfile_entry", opendb_keyfile_entry_);

	builder_->get_widget("opendb_cancel_btn", opendb_cancel_btn_);
	opendb_cancel_btn_->signal_clicked().connect(sigc::mem_fun(*this, &OpendbPage::on_cancel_button_clicked));

	builder_->get_widget("opendb_ok_btn", opendb_ok_btn_);
	opendb_ok_btn_->signal_clicked().connect(sigc::mem_fun(*this, &OpendbPage::on_ok_button_clicked));
}

void OpendbPage::go_to_page(std::string page_name)
{
	stack1_->set_visible_child(page_name);
}

void OpendbPage::clear_text_entries()
{
	opendb_password_entry_->set_text("");
	opendb_keyfile_entry_->set_text("");
}

void OpendbPage::on_cancel_button_clicked()
{
	clear_text_entries();
	go_to_page(invoker_page_name_);
}

void OpendbPage::on_ok_button_clicked()
{
	std::string password = opendb_password_entry_->get_text().data();

	if (!db_manager_->open_database(db_filename_, password))
  	{
  		std::cout << "Error while opening the database!" << std::endl;
  	}
  	else
  	{
  		std::cout << "Database opened!" << std::endl;
  		PasswordListManager::get_instance().fill_password_list();

  		// Set appropiate title to the main window
  		std::size_t last_ocurr = db_filename_.rfind('/');
		
		if (last_ocurr == std::string::npos)
			last_ocurr = db_filename_.rfind('\\');

		std::string db_short_name = db_filename_.substr(last_ocurr + 1);
		God::get_window()->set_title(db_short_name);

  		// Enable Close db and Add Password toolbar buttons
  		Gtk::ToolButton* add_pass_btn;
  		Gtk::ToolButton* close_db_btn;
  		
  		builder_->get_widget("add_pass", add_pass_btn);
  		add_pass_btn->set_sensitive(true);

  		builder_->get_widget("close_db", close_db_btn);
  		close_db_btn->set_sensitive(true);

  		clear_text_entries();
  		go_to_page(invoker_page_name_);
  	}
}