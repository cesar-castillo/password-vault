
#include "password_page.hpp"
#include "password_list_manager.hpp"
#include "god.hpp"
#include <iostream>

PasswordPage::PasswordPage(const std::string& invoker_page_name)
{
	builder_ = God::get_builder();
	db_manager_ = God::get_db_manager();
	invoker_page_name_ = invoker_page_name;

	// Initialize widgets and events
	initialize();
}

PasswordPage::~PasswordPage()
{	
	delete title_entry_;
	delete username_entry_;
	delete password_entry_;
	delete password2_entry_;
	delete url_entry_;
	delete description_entry_;
	delete cancel_button_;
	delete save_button_;
	// Dont delete because it is deleted on parent object (AppController)
	//delete stack1_;
}

void PasswordPage::show()
{
	go_to_page("password_page");
}

void PasswordPage::initialize()
{
	builder_->get_widget("stack1", stack1_);

	builder_->get_widget("title_entry", title_entry_);
	builder_->get_widget("username_entry", username_entry_);
	builder_->get_widget("password_entry", password_entry_);
	builder_->get_widget("password2_entry", password2_entry_);
	builder_->get_widget("url_entry", url_entry_);
	builder_->get_widget("description_entry", description_entry_);

	builder_->get_widget("cancel_pass_btn", cancel_button_);
	cancel_button_->signal_clicked().connect(sigc::mem_fun(*this, &PasswordPage::on_cancel_button_clicked));

	builder_->get_widget("save_pass_btn", save_button_);
	save_button_->signal_clicked().connect(sigc::mem_fun(*this, &PasswordPage::on_save_button_clicked));
}

void PasswordPage::uninitialize()
{
}

void PasswordPage::go_to_page(std::string page_name)
{
	stack1_->set_visible_child(page_name);
}

void PasswordPage::clear_text_entries()
{
	title_entry_->set_text("");
	username_entry_->set_text("");
	password_entry_->set_text("");
	password2_entry_->set_text("");
	url_entry_->set_text("");
	description_entry_->set_text("");
}

void PasswordPage::on_cancel_button_clicked()
{
	clear_text_entries();
	go_to_page(invoker_page_name_);
}

void PasswordPage::on_save_button_clicked()
{
	core::Entry entry;

	auto title = title_entry_->get_text();
	auto username = username_entry_->get_text();
	auto password = password_entry_->get_text();
	auto password2 = password2_entry_->get_text();
	auto url = url_entry_->get_text();
	auto description = description_entry_->get_text();

	// Validate that title, username and password are not empty
	if (title.length() == 0 || username.length() == 0 || password.length() == 0)
	{
		std::cout << "Important data is empty!" << std::endl;
		return;
	}

	// Validate that both passwords match
	if (password != password2)
	{
		std::cout << "Passwords do not match!" << std::endl;
		return;
	}

	entry.id = 0;
	strcpy(entry.title, title.data());
	strcpy(entry.username, username.data());
	strcpy(entry.password, password.data());
	strcpy(entry.website, url.data());
	strcpy(entry.description, description.data());

	if (!db_manager_->add_entry(entry))
	{
		std::cout << "There was an error while trying to add the password!" << std::endl;
	}
	else
	{
		std::cout << "Password added!" << std::endl;
		clear_text_entries();
		go_to_page(invoker_page_name_);
		// Refresh password list
		PasswordListManager::get_instance().fill_password_list();
		//Change window title to reflect db changes
		God::get_window()->set_title(God::get_window()->get_title() + '*');
		// Enable Save toolbar button
		Gtk::ToolButton* save_db_btn;
		builder_->get_widget("save_db", save_db_btn);
  		save_db_btn->set_sensitive(true);
	}
}