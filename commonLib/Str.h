#pragma once

#include <string>

namespace str{

	std::wstring s2ws(const char* s);

	string ws2s(const wchar_t* ws);

	bool isuft8(const char* s);

	string uft82gbk(const char* s);

	string gbk2utf8(const char* s);

	string urlEncode(const char* s);

	unsigned char ToHex(unsigned char x);

	unsigned char FromHex(unsigned char x);

	string urlDecode(const char* s);

};
