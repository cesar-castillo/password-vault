
#ifndef PASSWORD_LIST_MANAGER_H_
#define PASSWORD_LIST_MANAGER_H_

#include <gtkmm.h>
#include "pvdbmanager/db_manager.hpp"

class PasswordListManager
{
public:
	static PasswordListManager& get_instance();
	void fill_password_list();
	void clear_password_list();

private:
	PasswordListManager();
	PasswordListManager(const PasswordListManager&);
    PasswordListManager& operator=(const PasswordListManager&);
    ~PasswordListManager();
	
	Glib::RefPtr<Gtk::Builder> builder_;
	std::shared_ptr<core::DbManager> db_manager_;
};

#endif
