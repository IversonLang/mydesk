//source file: base64.cpp   

/* 
* RFC 1521 base64 encoding/decoding 
* 
* Copyright (C) 2006-2007 Christophe Devine 
* 
* This library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public 
* License, version 2.1 as published by the Free Software Foundation. 
* 
* This library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* Lesser General Public License for more details. 
* 
* You should have received a copy of the GNU Lesser General Public 
* License along with this library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
* MA 02110-1301 USA 
*/
#include "base64.h"   

static const unsigned char base64_enc_map[64] =  
{  
// 	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',  
// 	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',  
// 	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',  
// 	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  
// 	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',  
// 	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',  
// 	'8', '9', '-', '_'  

	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',  
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',  
	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',  
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',  
	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',  
	'8', '9', '+', '/'
};  

static const unsigned char base64_dec_map[128] =  
{  
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  
	127, 127, 127, 127, 127, 127, 127, 127, 127, 127,  
	127, 127, 127, 62, 127, 127, 127, 63, 52, 53,  
	54, 55, 56, 57, 58, 59, 60, 61, 127, 127,  
	127, 64, 127, 127, 127, 0, 1, 2, 3, 4,  
	5, 6, 7, 8, 9, 10, 11, 12, 13, 14,  
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  
	25, 127, 127, 127, 127, 127, 127, 26, 27, 28,  
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38,  
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48,  
	49, 50, 51, 127, 127, 127, 127, 127  
};  

/* 
* Encode a buffer into base64 format 
*/  
int base64_encode(const unsigned char *src, int slen,unsigned char *dst, DWORD *dlen)  
{  
	int i, n;  
	int C1, C2, C3;  
	unsigned char *p;  

	if( slen == 0 )  
		return( 0 );  

	n = (slen << 3) / 6;  

	switch( (slen << 3) - (n * 6) )  
	{  
	case 2: n += 3; break;  
	case 4: n += 2; break;  
	default: break;  
	}  

	if( *dlen < n + 1 )  
	{  
		*dlen = n + 1;  
		return( XYSSL_ERR_BASE64_BUFFER_TOO_SMALL );  
	}  

	n = (slen / 3) * 3;  

	for( i = 0, p = dst; i < n; i += 3 )  
	{  
		C1 = *src++;  
		C2 = *src++;  
		C3 = *src++;  

		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];  
		*p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];  
		*p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];  
		*p++ = base64_enc_map[C3 & 0x3F];  
	}  

	if( i < slen )  
	{  
		C1 = *src++;  
		C2 = ((i + 1) < slen) ? *src++ : 0;  

		*p++ = base64_enc_map[(C1 >> 2) & 0x3F];  
		*p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];  

		if( (i + 1) < slen )  
			*p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];  
		else *p++ = '=';  

		*p++ = '=';  
	}  
	*dlen =static_cast<int>(p - dst);  
	*p = 0;  
	return( 0 );  
}  

/* 
* Decode a base64-formatted buffer 
*/  
int base64_decode(const unsigned char *src, int slen,unsigned char *dst, DWORD *dlen)  
{  
	int i, j, n;  
	unsigned long x;  
	unsigned char *p;  

	for( i = j = n = 0; i < slen; i++ )  
	{  
		if( ( slen - i ) >= 2 &&  
			*(src+i) == '/r' && *(src+i+1) == '/n' )  
			continue;  

		if(*(src+i) == '/n' )  
			continue;  

		if(*(src+i) == '=' && ++j > 2 )  
			return( XYSSL_ERR_BASE64_INVALID_CHARACTER );  

		if(*(src+i) > 127 || base64_dec_map[*(src+i)] == 127 )  
			return( XYSSL_ERR_BASE64_INVALID_CHARACTER );  

		if( base64_dec_map[*(src+i)] < 64 && j != 0 )  
			return( XYSSL_ERR_BASE64_INVALID_CHARACTER );  

		n++;  
	}  

	if( n == 0 )  
		return( 0 );  

	n = ((n * 6) + 7) >> 3;  

	if( *dlen < n )  
	{  
		*dlen = n;  
		return( XYSSL_ERR_BASE64_BUFFER_TOO_SMALL );  
	}  

	for( j = 3, n = x = 0, p = dst; i > 0; i--, src++ )  
	{  
		if( *src == '/r' || *src == '/n' )  
			continue;  

		j -= ( base64_dec_map[*src] == 64 );  
		x = (x << 6) | ( base64_dec_map[*src] & 0x3F );  

		if( ++n == 4 )  
		{  
			n = 0;  
			if( j > 0 ) *p++ = (unsigned char)( x >> 16 );  
			if( j > 1 ) *p++ = (unsigned char)( x >> 8 );  
			if( j > 2 ) *p++ = (unsigned char)( x );  
		}  
	}  

	*dlen =static_cast<int>(p - dst);  

	return( 0 );  
}  


BOOL CBase64::Encrypt(const unsigned char *pSrc,int iSlen,unsigned char *pDst,DWORD *iDlen,string &strErrorInfo)  
{  
	strErrorInfo="";  
	int iRet=base64_encode(pSrc,iSlen,pDst,iDlen);  
	if(iRet==XYSSL_ERR_BASE64_BUFFER_TOO_SMALL)  
		strErrorInfo="分配的缓冲区太小!";  
	else if(iRet==XYSSL_ERR_BASE64_INVALID_CHARACTER)  
		strErrorInfo="无效数据!";  
	return iRet==0;   
}  
BOOL CBase64::Decrypt(const unsigned char *pSrc,int iSlen,unsigned char *pDst,DWORD *iDlen,string &strErrorInfo)  
{  
	strErrorInfo="";  
	int iRet=base64_decode(pSrc,iSlen,pDst,iDlen);  
	if(iRet==XYSSL_ERR_BASE64_BUFFER_TOO_SMALL)  
		strErrorInfo="分配的缓冲区太小!";  
	else if(iRet==XYSSL_ERR_BASE64_INVALID_CHARACTER)  
		strErrorInfo="无效数据!";
	return iRet==0;   
}  