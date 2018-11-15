
#include "god.hpp"

// Definition (after declaration inside class definition)
// of static data members
Glib::RefPtr<Gtk::Builder> God::builder_;
Gtk::Window* God::window_ = nullptr;
std::string God::current_db_filename_;