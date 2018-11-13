
#ifndef DATABASE_H_
#define DATABASE_H_

namespace core
{
	namespace constants
	{
		// Crypto constants
		const unsigned short PBKDF2_ITER_COUNT = 10000;
		const unsigned short SALT_LENGTH = 16;
		const unsigned short IV_LENGTH = 16;
		const unsigned short DEFAULT_KEY_LENGTH = 16;
	}

	struct PBKDF2Params
	{
		unsigned char salt[constants::SALT_LENGTH];
		unsigned short c;
	};

	struct Header
	{
		char db_name[40];
		PBKDF2Params pbkdf2_params;
		unsigned short key_length;
		unsigned char iv[constants::IV_LENGTH];
	};

	struct Entry
	{
		unsigned int id;
		char title[60];
		char username[40];
		char password[256];
		char description[512];
		char website[128];
	};

	struct Database
	{
		Header header;
		std::vector<Entry> entries;
	};
}

#endif