
#ifndef UTIL_H_
#define UTIL_H_

#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>

namespace crypto
{
	class Util
	{
	public:
		static void derive_key(byte* key, size_t key_length, byte* password, byte* salt, size_t salt_len, unsigned short c)
		{
			size_t pass_len = std::strlen((const char*) password);

			CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA512> pbkdf2;
			pbkdf2.DeriveKey(key, key_length, 0, password, pass_len, salt, salt_len, c);
		}

		static std::string to_hex(byte* data, size_t size)
		{
			std::string result;
			CryptoPP::StringSource ss(data, size, true,
				new CryptoPP::HexEncoder(new CryptoPP::StringSink(result))
			);

			return result;
		}

		static void from_hex(const std::string& data, size_t size, byte* result)
		{
			CryptoPP::StringSource ss(data, true,
				new CryptoPP::HexDecoder(new CryptoPP::ArraySink(result, size))
			);
		}

		static std::string to_base64(byte* data, size_t size)
		{
			std::string result;
			CryptoPP::StringSource ss(data, size, true,
				new CryptoPP::Base64Encoder(new CryptoPP::StringSink(result))
			);

			return result;
		}

		static void gen_random_bytes(byte* random_bytes, size_t size)
		{
			CryptoPP::AutoSeededRandomPool rnd;
			rnd.GenerateBlock(random_bytes, size);
		}	
	};
	
}

#endif