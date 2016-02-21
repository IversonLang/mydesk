#include "des.h"
#include <string.h>

CDes::CDes()
{
}
void CDes::GetDesKey(unsigned char *pbyKey, int n)
{
	memset(pbyKey, 0, 8);
	for(int i=0; i<n; i++)
	{
		Des(pbyKey, pbyKey, pbyKey, 'E');
	}
}

void  CDes:: DesMac(unsigned char *source, int iLen, unsigned char *dest,
            unsigned char *key)
{
    unsigned char strXOR[8], strTemp[8];
    int i, iCount=iLen/8, iRemain=iLen%8;
    memset(strXOR, 0, sizeof(strXOR));
    for(i=0; i<iCount; i++)
        Do_XOR(strXOR, source+8*i, 8);
    if (iRemain)
    {
        memset(strTemp, 0, 8);
        memcpy(strTemp, source+8*iCount, iRemain);
        Do_XOR(strXOR, strTemp, 8);
    }
    Des(strXOR, dest, key, 'e');
}


char * CDes::Enscrypt(const char* src,int len,const char* key,unsigned int* olen) //加密字符串文件
{
    unsigned char dstblock[8];
    unsigned char srcblock[8];
    unsigned char ukey[8];
    char* dst = NULL;
    memset(ukey,0,8);
    strncpy((char*)ukey,key,8);

    *olen = (len+7)/8*8;
    dst = new char[*olen + 1];
    memset(dst,0,*olen + 1);

    const char* pi = src;
    char* po = dst;
	while(len>0){
        if (len>8){
            memcpy(srcblock,pi,8);
            memset(dstblock,0,8);
            pi += 8;
            len -= 8;
            Des(srcblock,dstblock,ukey,0);
            memcpy(po,dstblock,8);
            po += 8;
        }else{
            memset(srcblock,0,8);
            memcpy(srcblock,pi,len);
            Des(srcblock,dstblock,ukey,0);
            memcpy(po,dstblock,8);
            len = 0;
        }
	}
    return  dst;
}


char * CDes::Descrypt(const char* src,int len,const char* key,unsigned int* olen ) 
{
    unsigned int ntlen = 0;
    unsigned char ukey[8];
	unsigned char dstblock[8];
	unsigned char srcblock[8];
    char* dst = new char[len+1];
    memset(dst,0,len+1);
    memset(ukey,0,8);
    strncpy((char*)ukey,key,8);

    const char* pi = src;
    char* po = dst;
	while(len>0){
        
        memcpy(srcblock,pi,8);
        pi += 8;
        len -= 8;

        memset(dstblock,0,8);
        Des(srcblock,dstblock,ukey,1);	
        if (len>0){
            memcpy(po,dstblock,8);
            po += 8;
            ntlen += 8;
        }  
	}
    //判断末尾是否被填充
    int i = 0;
    if(dstblock[7] < 8){
        for(i = 8 - dstblock[7]; i < 7; i++){
            if(dstblock[i] != '\0'){
                break;
            }
        }
    }	
    //有填充
    if(i == 7){
        memcpy(po,dstblock,8 - dstblock[7]);
        ntlen += 8 - dstblock[7];
    }
    //无填充
    else{
        memcpy(po,dstblock,8);
        ntlen += 8;
    }
    *olen = ntlen;
	return dst;
}
// 做DES
 void  CDes:: Des(unsigned char *source, unsigned char *dest,
	 unsigned char *key, unsigned char flag)
{
	char tmp[64];

	expand0(key, tmp);
	setkeystar((unsigned char *)tmp);
	if ( flag == ENCRYPT || flag == 'e' || flag == 'E' )
		encrypt0(source, dest);
	else
	    discrypt0(source, dest);
}

// 做异或
 void  CDes:: Do_XOR(unsigned char *dest, unsigned char *source, int size)
{
   int i;
   for(i=0; i<size; i++)
       dest[i] ^= source[i];
}

 void  CDes:: HEX_2_DSP(unsigned char *hex, unsigned char *dsp, int count)
{	//保证在同一串中操作成立
    int i;	
    unsigned char ch1, ch2;

    for (i = count-1; i >= 0; i--)
    {
        ch1=((hex[i]&0xf0)>>4);
        ch2=(hex[i]&0xf);
        dsp[i*2]=(ch1>9) ? ch1+0x41-10 : ch1+0x30;
        dsp[i*2+1]=(ch2>9) ? ch2+0x41-10 : ch2+0x30;
    }
}
    
 void  CDes:: DSP_2_HEX(unsigned char *dsp, unsigned char *hex, int count)
{
    int i;
    unsigned char ch1, ch2;

    for (i = 0; i < count; i++)
    {
        ch1 = dsp[i*2] - 0x30;
		ch2 = dsp[i*2+1] -0x30;
        hex[i] = (((ch1<=0x09) ? ch1 : (ch1&0x0F)+9) << 4 ) |
			      ((ch2<=0x09) ? ch2 : (ch2&0x0F)+9);
    }
}


void CDes:: encrypt0(unsigned char *text, unsigned char *mtext)
{
  char ll[64],rr[64],LL[64],RR[64];
  unsigned char tmp[64];
  int i,j;

  ip(text,(unsigned char *)ll,(unsigned char *)rr);

  for (i=1;i<17;i++)
  {
    _FFT(i,ll,rr,LL,RR);
    for (j=0;j<32;j++)
    {
      ll[j]=LL[j];
      rr[j]=RR[j];
    }
  }

  _ip(tmp,(unsigned char *)rr,(unsigned char *)ll);

  compress0(tmp,mtext);
}

void CDes:: discrypt0(unsigned char *mtext, unsigned char *text)
{
  char ll[64],rr[64],LL[64],RR[64];
  unsigned char tmp[64];
  int i,j;

  ip(mtext,(unsigned char *)ll,(unsigned char *)rr);

  for (i=16;i>0;i--)
  {
    _FFT(i,ll,rr,LL,RR);
    for (j=0;j<32;j++)
    {
      ll[j]=LL[j];
      rr[j]=RR[j];
    }
  }

  _ip(tmp,(unsigned char *)rr,(unsigned char *)ll);

  compress0(tmp,text);
}

void CDes:: expand0(unsigned char *in, char *out)
{
  int divide;
  int i,j;

  for (i=0;i<8;i++)
  {
    divide=0x80;
    for (j=0;j<8;j++)
    {
      *out++=(in[i]/divide)&1;
      divide/=2;
    }
  }
}

void CDes:: compress0(unsigned char *out, unsigned char *in)
{
  int times;
  int i,j;

  for (i=0;i<8;i++)
  {
    times=0x80;
    in[i]=0;
    for (j=0;j<8;j++)
    {
      in[i]+=(*out++)*times;
      times/=2;
    }
  }
}

void CDes:: compress016(unsigned char *out, unsigned char *in)
{
  int times;
  int i,j;

  for (i=0;i<16;i++)
  {
    times=0x8;
    in[i]='0';
    for (j=0;j<4;j++)
    {
      in[i]+=(*out++)*times;
      times/=2;
    }
  }
}

static int pc_1_c[28]=
{
	57,	49,	41,	33,	25,	17,	9,
	1,	58,	50,	42,	34,	26,	18,
	10,	2,	59,	51,	43,	35,	27,
	19,	11,	3,	60,	52,	44,	36
};
static int pc_1_d[28]=
{
	63,	55,	47,	39,	31,	23,	15,
	7,	62,	54,	46,	38,	30,	22,
	14,	6,	61,	53,	45,	37,	29,
	21,	13,	5,	28,	20,	12,	4
};
static int pc_2[48]=
{
	14,	17,	11,	24,	1,	5,
	3,	28,	15,	6,	21,	10,
	23,	19,	12,	4,	26,	8,
	16,	7,	27,	20,	13,	2,
	41,	52,	31,	37,	47,	55,
	30,	40,	51,	45,	33,	48,
	44,	49,	39,	56,	34,	53,
	46,	42,	50,	36,	29,	32
};

static int ls_count[16]=
{
  1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

void CDes:: setkeystar(unsigned char *bits)
{
	int i,j;

	for (i=0;i<28;i++)
		C[0][i]=bits[pc_1_c[i]-1];
	for (i=0;i<28;i++)
		D[0][i]=bits[pc_1_d[i]-1];
	for (j=0;j<16;j++)
	{
		LS((char *)C[j],(char *)C[j+1],ls_count[j]);
		LS((char *)D[j],(char *)D[j+1],ls_count[j]);
		son((char *)C[j+1],(char *)D[j+1],(char *)K[j+1]);
	}
}

void CDes:: LS(char *bits, char *buffer, int count)
{
	int i;

	for (i=0;i<28;i++)
	{
		buffer[i]=bits[(i+count)%28];
	}
}

void CDes:: son(char *cc, char *dd, char *kk)
{
	int i;
	char buffer[56];

	for (i=0;i<28;i++)
		buffer[i] = *cc++;

	for (i=28;i<56;i++)
		buffer[i] = *dd++;

	for (i=0;i<48;i++)
		*kk++=buffer[pc_2[i]-1];
}

static int ip_tab[64]=
{
	58,	50,	42,	34,	26,	18,	10,	2,
	60,	52,	44,	36,	28,	20,	12,	4,
	62,	54,	46,	38,	30,	22,	14,	6,
	64,	56,	48,	40,	32,	24,	16,	8,
	57,	49,	41,	33,	25,	17,	9,	1,
	59,	51,	43,	35,	27,	19,	11,	3,
	61,	53,	45,	37,	29,	21,	13,	5,
	63,	55,	47,	39,	31,	23,	15,	7
};
static int _ip_tab[64]=
{
	40,	8,	48,	16,	56,	24,	64,	32,
	39,	7,	47,	15,	55,	23,	63,	31,
	38,	6,	46,	14,	54,	22,	62,	30,
	37,	5,	45,	13,	53,	21,	61,	29,
	36,	4,	44,	12,	52,	20,	60,	28,
	35,	3,	43,	11,	51,	19,	59,	27,
	34,	2,	42,	10,	50,	18,	58,	26,
	33,	1,	41,	9,	49,	17,	57,	25
};

void CDes:: ip(unsigned char *text, unsigned char *ll, unsigned char *rr)
{
	int i;
	char buffer[64];

	expand0(text,buffer);

	for (i=0;i<32;i++)
		ll[i]=buffer[ip_tab[i]-1];

	for (i=0;i<32;i++)
		rr[i]=buffer[ip_tab[i+32]-1];
}

void CDes:: _ip(unsigned char *text, unsigned char *ll, unsigned char *rr)
{
	int i;
	char tmp[64];

	for (i=0;i<32;i++)
		tmp[i]=ll[i];
	for (i=32;i<64;i++)
		tmp[i]=rr[i-32];
	for (i=0;i<64;i++)
		text[i]=tmp[_ip_tab[i]-1];
}

static int e_r[48]=
{
	32,	1,	2,	3,	4,	5,	4,	5,	6,	7,	8,	9,
	8,	9,	10,	11,	12,	13,	12,	13,	14,	15,	16,	17,
	16,	17,	18,	19,	20,	21,	20,	21,	22,	23,	24,	25,
	24,	25,	26,	27,	28,	29,	28,	29,	30,	31,	32,	1
};

static int P[32]=
{
  16,7,20,21,29,12,28,17,
  1,15,23,26,5,18,31,10,
  2,8,24,14,32,27,3,9,
  19,13,30,6,22,11,4,25
};
static int SSS[16][4][16]=
{
  14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
  0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,	/* err on */
  4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
  15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,

  15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
  3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
  0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
  13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,

  10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
  13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
  13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
  1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,

  7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
  13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
  10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
  3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,	/* err on */

  2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
  14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, /* err on */
  4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
  11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,

  12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
  10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
  9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
  4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,

  4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
  13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
  1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
  6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,

  13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
  1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
  7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
  2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

void CDes:: _FFT(int n, char *ll, char *rr, char *LL, char *RR)
{
  int i;
  char buffer[64],tmp[64];

  for (i=0;i<48;i++)
	buffer[i]=rr[e_r[i]-1];
  for (i=0;i<48;i++)
	buffer[i]=(buffer[i]+K[n][i])&1;

  s_box(buffer,tmp);

  for (i=0;i<32;i++)
    buffer[i]=tmp[P[i]-1];

  for (i=0;i<32;i++)
    RR[i]=(buffer[i]+ll[i])&1;

  for (i=0;i<32;i++)
    LL[i]=rr[i];
}

void CDes:: s_box(char *aa, char *bb)
{
  int i,j,k,m;
  int y,z;
  char ss[8];

  m=0;
  for (i=0;i<8;i++)
  {
    j=6*i;
    y=aa[j]*2+aa[j+5];
    z=aa[j+1]*8+aa[j+2]*4+aa[j+3]*2+aa[j+4];
    ss[i]=SSS[i][y][z];
    y=0x08;
    for (k=0;k<4;k++)
    {
      bb[m++]=(ss[i]/y)&1;
      y/=2;
    }
  }
}

void CDes::Des_thre(unsigned char *source,unsigned char *dest,unsigned char  *key,char flag,int  leng )
{
	int i=0,j=0;
	unsigned char key1[8];
	unsigned char key2[8];
	unsigned char redest[8];
	for ( i=0;i<8;i++)
	{
		key1[i]=key[i];
	}
	i=0;
	for ( j=8;j<16;j++,i++)
	{
		key2[i]=key[j];
	}
	string retemp="";
	unsigned char temp[8]={0,0,0,0,0,0,0,0};
	  int k;
	  for ( i=0;i<leng;)
	  {
		  k=0;
		  memset(temp,0,8);
		  for (j=i,k=0;j<8,k<8;j++,k++,i++)
		  {
			  if (i<leng)
			  {
				  temp[k]=source[j];
			  }
			  
             
		  }
           if (flag==0)// 加密
           {
			Des(temp,redest,key1,0);
		    Des(redest,redest,key2,1);
		    Des(redest,redest,key1,0);

           }
	
		   else if (flag==1)
		   {
             Des(temp,redest,key1,1);
		     Des(redest,redest,key2,0);
		     Des(redest,redest,key1,1);
		   }
		 
		  for (k=0;k<8;k++)
		  {
			  retemp+=redest[k];
		  }
		  //AfxMessageBox(temp);
	  }
      int l=0;
	  l=leng%8;
	   if (l!=0)
	   {
		   l=8-l;
	   }
	   else
	   {
		   l=0;
	   }
	  for (i=0;i<leng+l;i++)
	  {
		  dest[i] = retemp[i];
	  }

}
void CDes::Cdes(char *source,  char *dest,
		  char *key,  char flag) //0 加密 1解密
{
		int i=0;
		unsigned char m_src[8];
		string strkey;	 
		strkey = source;
		for(  i=0;i<8;i++)
		{
			string h_str;
			h_str = strkey;
			strkey = h_str.substr(2);
			h_str = h_str.substr(0,2);
			unsigned char dwHex=0; 
			sscanf(h_str.c_str(),"%02X",&dwHex);
			m_src[i]=dwHex;
		}
		unsigned char m_srckey[8];
		string strkeykey;	 
		strkeykey = key;
		for( i=0;i<8;i++)
		{
			string h_str;
			h_str=strkeykey;
			strkeykey = h_str.substr(2);
			h_str = h_str.substr(0,2);
			unsigned char dwHex=0; 
			sscanf(h_str.c_str(),"%02X",&dwHex);
			m_srckey[i]=dwHex;
		}
		 unsigned char redest[8];
		Des((unsigned char*)m_src,(unsigned char*)redest,(unsigned char*)m_srckey,flag);
		  string str,tt;
		for ( i=0;i<8;i++)
		{
           str = redest[i];
		    tt+=str;
		}
		strcpy((char*)dest,tt.c_str());
	
}
void CDes::Des_thre_KEY(unsigned char *source,unsigned char *dest,unsigned char  *key,char flag,int  leng )
{

		unsigned char m_src[16];
		string strkey((char *)source);
		for( int i=0;i<16;i++)
		{
			string h_str;
			h_str=strkey;
			strkey = h_str.substr(2);
			h_str = h_str.substr(0,2);
			unsigned char dwHex=0; 
			sscanf(h_str.c_str(),"%02X",&dwHex);
			m_src[i]=dwHex;
		}
      Des_thre(m_src,dest,key,flag,leng);
}

