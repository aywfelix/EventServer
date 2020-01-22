#include "SFTest.h"
#include "LogUtil.h"
#include <vector>

// tea algorithm
namespace Encrypt
{
	typedef unsigned long  ULONG;
	typedef unsigned char byte;


	char intToHexChar(int x) {
		static const char HEX[16] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			 '8', '9', 'A', 'B',
			 'C', 'D', 'E', 'F'
		};
		return HEX[x];
	}

	int hexCharToInt(char hex) {
		hex = toupper(hex);
		if (isdigit(hex))
			return (hex - '0');
		if (isalpha(hex))
			return (hex - 'A' + 10);
		return 0;
	}

	string bytesToHexString(const byte* in, size_t size) {
		string str;
		for (size_t i = 0; i < size; ++i) {
			int t = in[i];
			int a = t / 16;
			int b = t % 16;
			str.append(1, intToHexChar(a));
			str.append(1, intToHexChar(b));
			if (i != size - 1)
				str.append(1, ' ');
		}
		return str;
	}

	size_t hexStringTobytes(const string& str, byte* out) {

		vector<string> vec;
		string::size_type currPos = 0, prevPos = 0;
		while ((currPos = str.find(' ', prevPos)) != string::npos) {
			string b(str.substr(prevPos, currPos - prevPos));
			vec.push_back(b);
			prevPos = currPos + 1;
		}
		if (prevPos < str.size()) {
			string b(str.substr(prevPos));
			vec.push_back(b);
		}
		typedef vector<string>::size_type sz_type;
		sz_type size = vec.size();
		for (sz_type i = 0; i < size; ++i) {
			int a = hexCharToInt(vec[i][0]);
			int b = hexCharToInt(vec[i][1]);
			out[i] = a * 16 + b;
		}
		return size;
	}


	class TEA {
	public:
		TEA(const byte* key, int round = 32, bool isNetbyte = false);
		TEA(const TEA& rhs);
		TEA& operator=(const TEA& rhs);
		void encrypt(const byte* in, byte* out);
		void decrypt(const byte* in, byte* out);
	private:
		void encrypt(const ULONG* in, ULONG* out);
		void decrypt(const ULONG* in, ULONG* out);
		ULONG ntoh(ULONG netlong) { return _isNetbyte ? ntohl(netlong) : netlong; }
		ULONG hton(ULONG hostlong) { return _isNetbyte ? htonl(hostlong) : hostlong; }
		inline double logbase(double base, double x) {
			return log(x) / log(base);
		}
	private:
		int _round;
		bool _isNetbyte;
		byte _key[16];
	};


	TEA::TEA(const byte* key, int round /*= 32*/, bool isNetbyte /*= false*/) :_round(round), _isNetbyte(isNetbyte)
	{
		if (key != 0)
			memcpy(_key, key, 16);
		else
			memset(_key, 0, 16);
	}

	TEA::TEA(const TEA& rhs) :_round(rhs._round), _isNetbyte(rhs._isNetbyte)
	{
		memcpy(_key, rhs._key, 16);
	}

	TEA& TEA::operator=(const TEA& rhs) {
		if (&rhs != this) {
			_round = rhs._round;
			_isNetbyte = rhs._isNetbyte;
			memcpy(_key, rhs._key, 16);
		}
		return *this;
	}

	void TEA::encrypt(const byte* in, byte* out) {
		encrypt((const ULONG*)in, (ULONG*)out);
	}

	void TEA::decrypt(const byte* in, byte* out) {
		decrypt((const ULONG*)in, (ULONG*)out);
	}

	void TEA::encrypt(const ULONG* in, ULONG* out) {

		ULONG* k = (ULONG*)_key;
		register ULONG y = ntoh(in[0]);
		register ULONG z = ntoh(in[1]);
		register ULONG a = ntoh(k[0]);
		register ULONG b = ntoh(k[1]);
		register ULONG c = ntoh(k[2]);
		register ULONG d = ntoh(k[3]);
		register ULONG delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
		register int round = _round;
		register ULONG sum = 0;

		while (round--) {    /* basic cycle start */
			sum += delta;
			y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
			z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
		}    /* end cycle */
		out[0] = ntoh(y);
		out[1] = ntoh(z);
	}

	void TEA::decrypt(const ULONG* in, ULONG* out) {

		ULONG* k = (ULONG*)_key;
		register ULONG y = ntoh(in[0]);
		register ULONG z = ntoh(in[1]);
		register ULONG a = ntoh(k[0]);
		register ULONG b = ntoh(k[1]);
		register ULONG c = ntoh(k[2]);
		register ULONG d = ntoh(k[3]);
		register ULONG delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
		register int round = _round;
		register ULONG sum = 0;

		if (round == 32)
			sum = 0xC6EF3720; /* delta << 5*/
		else if (round == 16)
			sum = 0xE3779B90; /* delta << 4*/
		else
			sum = delta << static_cast<int>(logbase(2, round));

		while (round--) {    /* basic cycle start */
			z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
			y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
			sum -= delta;
		}    /* end cycle */
		out[0] = ntoh(y);
		out[1] = ntoh(z);
	}
}

using namespace Encrypt;
void SFTest::TestEncypt()
{
	//static std::string key = "2@78346zz%&*(%^bq3hrty@#1";
	//int keylen = key.length();
	//std::string str = "nihao,thank you";
	//// encrypt
	//for (int i = 0; i < str.length(); ++i)
	//{
	//	str[i] = str[i] ^ (i % keylen);
	//}
	//// decrypt
	//for (int i = 0; i < str.length(); ++i)
	//{
	//	str[i] = str[i] ^ (i % keylen);
	//}
	//std::cout << str << std::endl;

	const string plainStr("AD DE E2 DB B3 E2 DB B3");
	const string keyStr("3A DA 75 21 DB E2 DB B3 11 B4 49 01 A5 C6 EA D4");
	const int SIZE_IN = 8, SIZE_OUT = 8, SIZE_KEY = 16;
	Encrypt::byte plain[SIZE_IN], crypt[SIZE_OUT], key[SIZE_KEY];
	Encrypt::TEA tea(key, 16, true);

	size_t size_in = hexStringTobytes(plainStr, plain);
	size_t size_key = hexStringTobytes(keyStr, key);
	tea.encrypt(plain, crypt);
	tea.decrypt(crypt, plain);
	cout << "Plain: " << bytesToHexString(plain, SIZE_IN) << endl;
}
