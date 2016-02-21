#include <string>
using namespace std;
#ifndef   _DES_H__
#define   _DES_H__


#ifndef   ENCRYPT
#define   ENCRYPT  0         /* DES 方向 */
#define   DECRYPT  1
#endif

#ifndef   STAND
#define   STAND    0         /* MAC 标准 */
#define   BPI      1
  
#endif
//#define   MAINKEY     "FDC92F6803DB16F1"
//#include "HttpCrc.h"
typedef unsigned int        UINT;

class CDes
{
public:
     CDes();
void GetDesKey(unsigned char *pbyKey, int n);
void encrypt0(unsigned char *text, unsigned char *mtext);
void discrypt0(unsigned char *mtext, unsigned char *text);
void expand0(unsigned char *in, char *out);
void compress0(unsigned char *out, unsigned char *in);
void compress016(unsigned char *out, unsigned char *in);

void setkeystar(unsigned char *bits);
void LS(char *bits, char *buffer, int count);
void son(char *cc, char *dd, char *kk);

void ip(unsigned char *text, unsigned char *ll, unsigned char *rr);
void _ip(unsigned char *text, unsigned char *ll, unsigned char *rr);
void _FFT(int n, char *ll, char *rr, char *LL, char *RR);
void s_box(char *aa, char *bb);
void Des_thre_KEY(unsigned char *source,unsigned char *dest,unsigned char  *key,char flag,int  leng );

/*
DES加密解密程序
参数:  source --- 源(8字节16进制)
dest   --- 目标(8字节16进制)
key    --- KEY值(8字节16进制)
flag   --- ENCRYPT 加密
DECRYPT 解密
*/

char * Enscrypt(const char* src,int len,const char* key,unsigned int* olen);
char * Descrypt(const char* src,int len,const char* key,unsigned int* olen);

void  Des(unsigned char *source, unsigned char *dest,
		  unsigned char *key, unsigned char flag);

void  DesMac(unsigned char *source, int iLen,
			 unsigned char *dest, unsigned char *key);
void  Do_XOR(unsigned char *dest, unsigned char *source, int size);
void  HEX_2_DSP(unsigned char *hex, unsigned char *dsp, int count);
void  DSP_2_HEX(unsigned char *dsp, unsigned char *hex, int count);
void  Cdes(char *source,  char *dest,char *key,  char flag); //0 加密 1解密
void  Des_thre(unsigned char *source,unsigned char *dest,unsigned char  *key,char flag,int  leng );

public:
 unsigned char  C[17][28];
 unsigned char  D[17][28];
 unsigned char  K[17][48];
 unsigned char  c;
 unsigned char  ch1;

 
};
#endif
