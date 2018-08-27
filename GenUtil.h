#pragma once


class CGenUtil  
{
public:
	static int HexToAsc(BYTE *pDst, char *pSrc, int nSrcLen);
	static int AscToHex(char *pDst,BYTE *pSrc,int SrcLen);
	static void OutputLog(int tab,char *fmt,...);
	static void Base64Decode(char *ps8Dest, const char *ps8Source, int s32InLen);
	static void Base64Encode(char *ps8Dest, const char *ps8Source, int s32InLen);
};


int isUsersPri();			// 获取程序 运行权限
