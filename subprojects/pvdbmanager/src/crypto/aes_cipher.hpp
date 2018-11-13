
#ifndef AES_CIPHER_H_
#define AES_CIPHER_H_

#include "cipher.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

#include <iostream>

namespace crypto
{

	class AESCipher : public Cipher
	{
	public:
		AESCipher(byte* key, unsigned short key_length, byte* iv, unsigned short iv_length);
		AESCipher(const std::string& password);
		AESCipher(const std::string& password, byte* salt, size_t salt_len, unsigned short iter_count = 10000);
		~AESCipher();
		
		void encrypt(const char* plain_text) override;
		void encrypt(const char* plain_text, byte* cipher_text) override;
		void encrypt(const char* plain_text, std::vector<byte>& cipher_text) override;
		void encrypt(const std::vector<char>& plain_text, std::vector<byte>& cipher_text) override;
		void encrypt_file(const char* file_name) override;
		
		void decrypt(const char* cipher_text) override;
		void decrypt(const byte* cipher_text, char* plain_text) override;
		void decrypt(const std::vector<byte>& cipher_text, std::vector<byte>& plain_text) override;
		void decrypt_file(const char* file_name) override;
	};
}

#endif