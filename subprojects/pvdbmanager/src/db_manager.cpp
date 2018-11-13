
#include <fstream>
#include <experimental/filesystem>
#include "pvdbmanager/db_manager.hpp"
#include "crypto/aes_cipher.hpp"
#include "crypto/util.hpp"

core::DbManager::DbManager() {}

core::DbManager::DbManager(std::string db_file_name)
{
	db_file_name_ = db_file_name;
}

bool core::DbManager::create_database(const std::string& db_name, const std::string& password)
{
	std::string full_db_file_name = db_name + "." + db_file_extension_;
	
	if (std::experimental::filesystem::exists(full_db_file_name))
	{
		std::cout << "File already exists\n";
		return false;
	}

	std::fstream file(full_db_file_name, std::ios::binary | std::ios::out);

	if (!file)
	{
		std::cout << "There was an error creating the database\n";
		return false;
	}

	// Fill database header
	Header db_header;
	// database name
	std::copy(db_name.begin(), db_name.end(), db_header.db_name);
	// Generate and set the salt for the pbkdf2 algorithm
	crypto::Util::gen_random_bytes((byte*)db_header.pbkdf2_params.salt,
		sizeof(db_header.pbkdf2_params.salt));
	// PBKDF2 iteration count
	db_header.pbkdf2_params.c = constants::PBKDF2_ITER_COUNT;
	// key length
	db_header.key_length = constants::DEFAULT_KEY_LENGTH;
	// initialization vector (iv)
	size_t iv_size = sizeof(db_header.iv);
	unsigned char iv[iv_size] = "IV";
	std::copy(iv, iv + iv_size, db_header.iv);

	// Then save the header to the file
	file.write((char*)&db_header, sizeof(db_header));
	file.close();

	return true;
}

bool core::DbManager::remove_database(const std::string& db_name)
{
	return true;
}

bool core::DbManager::open_database(const std::string& db_name, const std::string& password)
{
	std::fstream file(db_name, std::ios::binary | std::ios::in);

	if (!file)
	{
		std::cout << "There was an error opening the database." << std::endl;
		return false;
	}

	Database db;
	// Read the header of the database
	file.read((char*)&db.header, sizeof(db.header));
	
	file.seekg(0, std::ios::end);
	size_t total_size = file.tellg();

	if (total_size > sizeof(db.header))
	{
		file.seekg(sizeof(db.header), std::ios::beg);
		size_t raw_entry_data_size = total_size - sizeof(db.header);
		char* raw_entry_data = new char[raw_entry_data_size];
		file.read((char*)raw_entry_data, raw_entry_data_size);

		std::vector<byte> raw_entry_data_vec{ raw_entry_data, raw_entry_data + raw_entry_data_size };

		// Get parameters of PBKDF2 in the header
		byte* salt = (byte*)db.header.pbkdf2_params.salt;
		size_t salt_len = sizeof(db.header.pbkdf2_params.salt);

		crypto::AESCipher cipher(password, salt, salt_len);
		cipher.set_iv((byte*)db.header.iv, sizeof(db.header.iv));

		std::vector<byte> plain_entry_data;
		cipher.decrypt(raw_entry_data_vec, plain_entry_data);

		auto it = plain_entry_data.begin();
		auto it2 = it + sizeof(Entry);
		size_t entry_count = plain_entry_data.size() / sizeof(Entry);

		for (size_t i = 0; i < entry_count; i++)
		{
			char* data = new char[sizeof(Entry)];
			std::copy(it, it2, data);
			Entry* ent = (Entry*)data;
			db.entries.push_back(*ent);
			
			it = it2;
			it2 = it + sizeof(Entry);
			delete [] data;
		}

		delete [] raw_entry_data;
	}

	file.close();
	
	// Set members database object and file name
	database_ = std::make_unique<Database>(db);
	db_file_name_ = db_name;

	return true;
}

bool core::DbManager::close_database()
{
	database_.reset();
	return true;
}

bool core::DbManager::save_database(const std::string& password)
{
	// Return false if no database object exists
	if (!database_) return false;

	std::string full_db_file_name = db_file_name_ + "_tmp" + "." + db_file_extension_;
	std::fstream file(full_db_file_name, std::ios::binary | std::ios::out);

	if (!file)
	{
		std::cout << "There was an error saving the database." << std::endl;
		return false;
	}

	// Write the db header
	file.write((char*)&database_->header, sizeof(database_->header));

	char* raw_pointer = reinterpret_cast<char *>(database_->entries.data());
	size_t raw_data_len = database_->entries.size() * sizeof(Entry);
	std::vector<char> raw_entry_data{ raw_pointer, raw_pointer + raw_data_len };

	// Get parameters of PBKDF2
	byte* salt = (byte*)database_->header.pbkdf2_params.salt;
	size_t salt_len = sizeof(database_->header.pbkdf2_params.salt);

	crypto::AESCipher cipher(password, salt, salt_len);
	std::vector<byte> cipher_text;
	cipher.encrypt(raw_entry_data, cipher_text);

	file.write((char*)cipher_text.data(), cipher_text.size());

	//Update the initialization vector in the header section
	byte* iv = cipher.get_iv();
	size_t iv_position = sizeof(database_->header.db_name)
		+ sizeof(database_->header.pbkdf2_params)
		+ sizeof(database_->header.key_length);

	file.seekg(iv_position, std::ios::beg);
	file.write((char*)iv, sizeof(database_->header.iv));
	
	file.close();

	std::remove(db_file_name_.data());
	std::rename(full_db_file_name.data(), db_file_name_.data());

	return true;
}

bool core::DbManager::add_entry(Entry& entry)
{
	bool result = false;
	
	if (database_)
	{
		if (std::strlen(entry.title) > 0
			&& std::strlen(entry.username) > 0
			&& std::strlen(entry.password) > 0)
		{
			entry.id = get_last_entry_id() + 1;
			database_->entries.push_back(entry);
			result = true;
		}
	}

	return result;
}

bool core::DbManager::remove_entry(unsigned int id)
{
	bool result = false;
	auto pred = [id](const Entry& entry) {
		return entry.id == id;
	};

	auto it = std::find_if(database_->entries.begin(), database_->entries.end(), pred);

	if (it != database_->entries.end())
	{
		database_->entries.erase(it);
		result = true;
	}
	
	return result;
}

bool core::DbManager::update_entry(const Entry& entry)
{
	bool result = false;
	unsigned int entry_id = entry.id;
	
	auto pred = [entry_id](const Entry& ent) {
		return ent.id == entry_id;
	};

	auto it = std::find_if(database_->entries.begin(), database_->entries.end(), pred);

	if (it != database_->entries.end())
	{
		std::copy(entry.title, entry.title + sizeof(entry.title), (*it).title);
		std::copy(entry.username, entry.username + sizeof(entry.username), (*it).username);
		std::copy(entry.password, entry.password + sizeof(entry.password), (*it).password);
		std::copy(entry.description, entry.description + sizeof(entry.description), (*it).description);
		std::copy(entry.website, entry.website + sizeof(entry.website), (*it).website);

		result = true;
	}
	
	return result;
}

core::Entry core::DbManager::get_entry_by_id(unsigned int id) const
{
	bool find = false;
	auto pred = [id](const Entry& entry) {
		return entry.id == id;
	};

	auto it = std::find_if(database_->entries.begin(), database_->entries.end(), pred);

	if (it != database_->entries.end())
		find = true;

	if (find) return *it; else return Entry {};
}

std::vector<core::Entry> core::DbManager::get_all_entries() const
{
	return database_->entries;
}

unsigned int core::DbManager::get_last_entry_id()
{
	if (database_->entries.size() > 0)
		return database_->entries.back().id;
	else
		return 0;
}