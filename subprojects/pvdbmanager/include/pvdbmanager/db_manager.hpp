
#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include <iostream>
#include <string>
#include <vector>

#include "database.hpp"

namespace core
{
	class DbManager
	{
	public:
		DbManager();
		DbManager(std::string db_file_name);

		bool create_database(const std::string& db_name, const std::string& password);
		bool remove_database(const std::string& db_name);
		bool open_database(const std::string& db_name, const std::string& password);
		bool close_database();
		bool save_database(const std::string& password);
		
		bool add_entry(Entry& entry);
		bool remove_entry(unsigned int id);
		bool update_entry(const Entry& entry);
		Entry get_entry_by_id(unsigned int id) const;
		std::vector<Entry> get_all_entries() const;

	private:
		unsigned int get_last_entry_id();

	private:
		std::string db_file_name_;
		std::unique_ptr<Database> database_;

		const std::string db_file_extension_ = "dat";
	};
}

#endif