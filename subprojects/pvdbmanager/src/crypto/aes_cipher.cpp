
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include "aes_cipher.hpp"
#include "util.hpp"

crypto::AESCipher::AESCipher(byte* key, unsigned short key_length,
	byte* iv, unsigned short iv_length) : Cipher(key, key_length, iv, iv_length)
{

}

crypto::AESCipher::AESCipher(const std::string& password)
{
	using namespace CryptoPP;

	const unsigned short salt_len = 8;
	const unsigned short iter_count = 10000;

	// Set key length and iv length
	key_length_ = AES::DEFAULT_KEYLENGTH;
	iv_length_ = AES::BLOCKSIZE;

	byte* password_bytes = (byte*)password.data();
	
	// Generate salt in order to generate the key
	// This parameter must be saved, along the iteration count
	byte* salt = new byte[salt_len];
	Util::gen_random_bytes(salt, salt_len);
	
	// Generate and set key	
	key_ = new byte[key_length_];
	Util::derive_key(key_, key_length_, password_bytes, salt, salt_len, iter_count);

	// Set initialization vector (iv)
	iv_ = new byte[iv_length_];
	Util::gen_random_bytes(iv_, iv_length_);

	delete [] salt;
}

crypto::AESCipher::AESCipher(const std::string& password, byte* salt, size_t salt_len, unsigned short iter_count)
{
	using namespace CryptoPP;

	// Set key length and iv length
	key_length_ = AES::DEFAULT_KEYLENGTH;
	iv_length_ = AES::BLOCKSIZE;

	byte* password_bytes = (byte*)password.data();
	
	// Generate and set key	
	key_ = new byte[key_length_];
	Util::derive_key(key_, key_length_, password_bytes, salt, salt_len, iter_count);

	// Set initialization vector (iv)
	iv_ = new byte[iv_length_];
	Util::gen_random_bytes(iv_, iv_length_);
}

crypto::AESCipher::~AESCipher()
{
}

void crypto::AESCipher::encrypt(const char* plain_text)
{
	using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);
	
	//AutoSeededRandomPool rnd;
	//rnd.GenerateBlock(iv, iv.size());

	CFB_Mode<AES>::Encryption enc(key, key.size(), iv);
	
	std::string cipher_text;
	StringSource ss(plain_text, true,
		new StreamTransformationFilter(enc, new StringSink(cipher_text))
	);
}

void crypto::AESCipher::encrypt(const char* plain_text, byte* cipher_text)
{
	/*using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);
	
	//AutoSeededRandomPool rnd;
	//rnd.GenerateBlock(iv, iv.size());
	HexEncoder encoder(new FileSink(std::cout));

	CFB_Mode<AES>::Encryption enc(key, key.size(), iv);
	
	std::vector<byte> plain_text2;
	std::copy(plain_text, plain_text + strlen(plain_text), std::back_inserter(plain_text2));

	//std::vector<byte> cipher_text2;
	cipher_text.resize(plain_text2.size() + AES::BLOCKSIZE);
  	ArraySink as(&cipher_text[0], cipher_text.size());

	ArraySource ss(plain_text2.data(), plain_text2.size(), true,
		new StreamTransformationFilter(enc, new Redirector(as))
	);

	cipher_text.resize(as.TotalPutLength());*/
}

void crypto::AESCipher::encrypt(const char* plain_text, std::vector<byte>& cipher_text)
{
	using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);

	CFB_Mode<AES>::Encryption enc(key, key.size(), iv);
	
	std::vector<byte> plain_text2;
	std::copy(plain_text, plain_text + strlen(plain_text), std::back_inserter(plain_text2));

	cipher_text.resize(plain_text2.size() + AES::BLOCKSIZE);
  	ArraySink as(&cipher_text[0], cipher_text.size());

	ArraySource ss(plain_text2.data(), plain_text2.size(), true,
		new StreamTransformationFilter(enc, new Redirector(as))
	);

	cipher_text.resize(as.TotalPutLength());
}

void crypto::AESCipher::encrypt(const std::vector<char>& plain_text, std::vector<byte>& cipher_text)
{
	using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);

	CFB_Mode<AES>::Encryption enc(key, key.size(), iv);

	cipher_text.resize(plain_text.size() + AES::BLOCKSIZE);
  	ArraySink as(&cipher_text[0], cipher_text.size());

	ArraySource ss((byte*)plain_text.data(), plain_text.size(), true,
		new StreamTransformationFilter(enc, new Redirector(as))
	);

	cipher_text.resize(as.TotalPutLength());
}

void crypto::AESCipher::encrypt_file(const char* file_name)
{
	using namespace CryptoPP;

	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);

	CFB_Mode<AES>::Encryption enc(key, key.size(), iv);

	FileSource fs(file_name, true,
		new StreamTransformationFilter(enc, new FileSink("passwords_encrypted.dat"))
	);
}

void crypto::AESCipher::decrypt(const char* cipher_text)
{
	using namespace CryptoPP;

	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);

	CFB_Mode<AES>::Decryption dec(key, key.size(), iv);

	std::string plain_text;
	StringSource ss(cipher_text, true,
		new StreamTransformationFilter(dec, new StringSink(plain_text))
	);
}

void crypto::AESCipher::decrypt(const byte* cipher_text, char* plain_text)
{
	using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);

	CFB_Mode<AES>::Decryption dec(key, key.size(), iv);
	
	StringSource ss(cipher_text, true,
		new StreamTransformationFilter(dec, new ArraySink((byte*)plain_text, sizeof(plain_text)))
	);
}

void crypto::AESCipher::decrypt(const std::vector<byte>& cipher_text, std::vector<byte>& plain_text)
{
	using namespace CryptoPP;

	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);
	
	plain_text.resize(cipher_text.size());
	ArraySink as(&plain_text[0], plain_text.size());

	CFB_Mode<AES>::Decryption dec(key, key.size(), iv);
	
	ArraySource ss(cipher_text.data(), cipher_text.size(), true,
		new StreamTransformationFilter(dec, new Redirector(as))
	);

	plain_text.resize(as.TotalPutLength());
}

void crypto::AESCipher::decrypt_file(const char* file_name)
{
	using namespace CryptoPP;
	
	SecByteBlock key(key_, AES::DEFAULT_KEYLENGTH);
	SecByteBlock iv(iv_, AES::BLOCKSIZE);
	//AutoSeededRandomPool rnd;
	//rnd.GenerateBlock(iv, iv.size());

	CFB_Mode<AES>::Decryption dec(key, key.size(), iv);

	FileSource fs(file_name, true,
		new StreamTransformationFilter(dec, new FileSink("passwords_decrypted.dat"))
	);
}