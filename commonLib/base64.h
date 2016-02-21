#ifndef XYSSL_BASE64_H   
#define XYSSL_BASE64_H   

#define XYSSL_ERR_BASE64_BUFFER_TOO_SMALL -0x0010   
#define XYSSL_ERR_BASE64_INVALID_CHARACTER -0x0012   

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
using    namespace    std;

#define SAFE_DELETE(ptr) do{ if(ptr) {delete ptr; ptr=NULL;} }while(0)
#define SAFE_INITLP(a,b,c)b = new a[c]; memset(b,0,sizeof(a) * c);
int base64_encode(const unsigned char *src, int slen,unsigned char *dst, DWORD *dlen);
int base64_decode(const unsigned char *src, int slen,unsigned char *dst, DWORD *dlen);

class CBase64    
{  
public:  
	CBase64();  
	virtual ~CBase64();  
	static BOOL Encrypt(const unsigned char *pSrc,int iSlen,unsigned char *pDst,DWORD *iDlen,string &strErrorInfo);  
	static BOOL Decrypt(const unsigned char *pSrc,int iSlen,unsigned char *pDst,DWORD *iDlen,string &strErrorInfo);  
private:  

};
#endif