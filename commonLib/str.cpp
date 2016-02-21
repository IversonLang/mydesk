#include "CommonLib.h"

std::wstring str::s2ws(const char* s)
{
    int nStrLen = strlen(s);
    wstring ws;
    ws.resize(nStrLen);
    MultiByteToWideChar(CP_ACP,0,s,nStrLen,(WCHAR*)ws.c_str(),ws.size());
    return ws;
}
string str::ws2s(const wchar_t* ws)
{
    int nStrLen = wcslen(ws);
    string s;
    s.resize(nStrLen*2);
    WideCharToMultiByte(CP_ACP,0,ws,nStrLen,(char*)s.c_str(),s.size(),0,0);
    return s;
}

bool str::isuft8(const char* s)
{
    int encodingBytesCount = 0;
    bool allTextsAreASCIIChars = true;
    int nStrLen = strlen(s);

    for (int i = 0; i < nStrLen; i++){

        byte current = s[i];
        if ((current & 0x80) == 0x80){                    
            allTextsAreASCIIChars = false;
        }

        if (encodingBytesCount == 0){
            if ((current & 0x80) == 0){
                continue;
            }

            if ((current & 0xC0) == 0xC0){
                encodingBytesCount = 1;
                current <<= 2;
                while ((current & 0x80) == 0x80){
                    current <<= 1;
                    encodingBytesCount++;
                }
            }else{
                return false;
            }
        }else{
            if ((current & 0xC0) == 0x80){     
                encodingBytesCount--;
            }else{
                return false;
            }
        }
    }
    if (encodingBytesCount != 0){
        return false;
    }
    return !allTextsAreASCIIChars;
}

string str::uft82gbk(const char* s)
{
    if(!str::isuft8(s)){
        return s;
    }
    int len = ::MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);  
    if (len == 0){
        return "";
    }

    wstring ws;
    ws.resize(len);
    ::MultiByteToWideChar(CP_UTF8, 0, s, -1, (WCHAR*)ws.c_str(), len);  

    return ws2s(ws.c_str());  
}

string str::gbk2utf8(const char* s)
{
	wstring ws = s2ws(s);

	int len = WideCharToMultiByte(CP_UTF8,0,ws.c_str(),-1,0,0,0,0);
	
	string sutf8;
	sutf8.resize(len);

	WideCharToMultiByte(CP_UTF8,0,ws.c_str(),-1,(char*)sutf8.c_str(),len,0,0);

	return sutf8;
}

string str::urlEncode(const char* s)
{
	if (s == NULL){
		return "";
	}
	std::string strTemp = "";
	size_t length = strlen(s);
	for (size_t i = 0; i < length; i++){
		if (isalnum((unsigned char)s[i]) || 
			(s[i] == '-') ||
			(s[i] == '_') || 
			(s[i] == '.') || 
			(s[i] == '~')){
			strTemp += s[i];
		}else if (s[i] == ' '){
			strTemp += "+";
		}else{
			strTemp += '%';
			strTemp += str::ToHex((unsigned char)s[i] >> 4);
			strTemp += str::ToHex((unsigned char)s[i] % 16);
		}
	}
	return strTemp;
}

unsigned char str::ToHex(unsigned char x) 
{ 
	return  x > 9 ? x + 55 : x + 48; 
}

unsigned char str::FromHex(unsigned char x) 
{ 
	unsigned char y;
	if (x >= 'A' && x <= 'Z'){
		y = x - 'A' + 10;
	}else if(x >= 'a' && x <= 'z'){
		y = x - 'a' + 10;
	}else if(x >= '0' && x <= '9'){
		y = x - '0';
	}else{
		y = '0';
	}
	return y;
}


string str::urlDecode(const char* s)
{
	if (s == NULL){
		return "";
	}
	std::string strTemp = "";
	size_t length = strlen(s);
	for (size_t i = 0; i < length; i++){
		if (s[i] == '+'){
			strTemp += ' ';
		}else if (s[i] == '%'){
			if(i + 2 < length){
				unsigned char high = str::FromHex((unsigned char)s[++i]);
				unsigned char low = str::FromHex((unsigned char)s[++i]);
				strTemp += high*16 + low;
			}
		}else{
			strTemp += s[i];
		}
	}
	return strTemp;
}