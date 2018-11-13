
#ifndef CIPHER_H_
#define CIPHER_H_

#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>

#include <cstring>

#include <iostream>

namespace crypto
{
	class Cipher
	{
	public:
		Cipher() {}

		Cipher(byte* key, unsigned short key_length, byte* iv, unsigned short iv_length)
		{
			key_ = key;
			key_length_ = key_length;
			iv_ = iv;
			iv_length_ = iv_length;
		}
		
		virtual ~Cipher()
		{
			delete [] key_;
			delete [] iv_;
		}

		void set_key(byte* key)
		{
			key_ = key;
		}

		void set_iv(byte* iv)
		{
			iv_ = iv;
		}

		void set_iv(byte* iv, size_t size)
		{
			std::copy(iv, iv + size, iv_);
		}

		byte* get_iv()
		{
			return iv_;
		}

		virtual void encrypt(const char* plain_text) = 0;
		virtual void encrypt(const char* plain_text, byte* cipher_text) = 0;
		virtual	void encrypt(const char* plain_text, std::vector<byte>& cipher_text) = 0;
		virtual void encrypt(const std::vector<char>& plain_text, std::vector<byte>& cipher_text) = 0;
		virtual void encrypt_file(const char* file_name) = 0;

		virtual void decrypt(const char* cipher_text) = 0;
		virtual void decrypt(const byte* cipher_text, char* plain_text) = 0;
		virtual void decrypt(const std::vector<byte>& cipher_text, std::vector<byte>& plain_text) = 0;
		virtual void decrypt_file(const char* file_name) = 0;

	protected:
		byte* key_;
		unsigned short key_length_;
		byte* iv_;
		unsigned short iv_length_;
	};
}

#endif