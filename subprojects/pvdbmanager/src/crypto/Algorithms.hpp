
namespace crypto
{
	enum class Algorithms
	{
		AES,
		BLOWFISH,
		3DES
	}

	enum class AESOpModes
	{
		ECB,
		CBC,
		CFB,
		OFB,
		CTR,
		XTS,
		GCM
	}
}