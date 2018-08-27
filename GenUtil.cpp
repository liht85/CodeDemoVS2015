#include "StdAfx.h"
#include "GenUtil.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CGenUtil::HexToAsc(BYTE *pDst, char *pSrc, int nSrcLen)
{
	for(int i=0; i<nSrcLen; i+=2)
	{
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else if(*pSrc>='A' && *pSrc<='F')
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}
		else
		{
			*pDst = (*pSrc - 'a' + 10) << 4;
		}
		
		pSrc++;
	
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst |= *pSrc - '0';
		}
		else if(*pSrc>='A' && *pSrc<='F')
		{
			*pDst |= *pSrc - 'A' + 10;
		}
		else
		{
			*pDst |= *pSrc - 'a' + 10;
		}
		
		pSrc++;
		pDst++;
	}
	return nSrcLen / 2;
}

int CGenUtil::AscToHex(char *pDst,BYTE *pSrc,int SrcLen)
{	
	const char tab[]="0123456789ABCDEF";
	
	for(int i=0; i<SrcLen; i++)
	{
		*pDst++ = tab[*((unsigned char*)pSrc) >> 4];	
		*pDst++ = tab[*((unsigned char*)pSrc) & 0x0f];	
		pSrc++;
	}
	
	*pDst = '\0';
	return SrcLen * 2;
}


int String2Bytes(char * pSrc, int nSrcLen, PBYTE pDst, int* pnDstLen)
{
    int i=0;
    char cVal1 = 0;
    char cVal2 = 0;
    char cByte;
    char *pSrcTmp = 0;
    if (pDst != NULL){
        *pDst = 0;
    }
    if (pSrc == NULL || nSrcLen <= 0 ||  nSrcLen%2 == 1 ||
        pDst == NULL ){
        return -1;
    }
    if(0 == strncmp(pSrc,"0x",2) || 0 == strncmp(pSrc,"0X",2)){
        pSrcTmp = pSrc + 2;
        nSrcLen -= 2;
    }else{
        pSrcTmp = pSrc;
    }
	
    //const char szTable[] = "0123456789ABCDEF";
    for (i=0 ; i<nSrcLen/2; i++){
        cByte = pSrcTmp[i*2];
		
        if((cByte >= '0') && (cByte <= '9')){
            cVal1 = cByte - 0x30;
        }else if((cByte >= 'A') && (cByte <= 'F')){
            cVal1 = cByte - 0x37;
        }else if((cByte >= 'a') && (cByte <= 'f')){
            cVal1 = cByte - 0x57;
        }else{
            return -1;
        }
        cByte = pSrcTmp[i*2+1];
        if((cByte >= '0') && (cByte <= '9')){
            cVal2 = cByte - 0x30;
        }else if((cByte >= 'A') && (cByte <= 'F')){
            cVal2 = cByte - 0x37;
        }else if((cByte >= 'a') && (cByte <= 'f')){
            cVal2 = cByte - 0x57;
        }else{
            return -1;
        }
        pDst[i] = cVal1 *16 + cVal2;
    }
    *pnDstLen = nSrcLen/2;
    return  0;
}


int Bytes2String(PBYTE pSrc, int nSrcLen, char *szDst, int nDstLenMax)
{
    int i=0;
    if (szDst != NULL)
    {
        *szDst = 0;
    }
    if (pSrc == NULL || nSrcLen <= 0 || szDst == NULL || nDstLenMax <= nSrcLen*2)
    {
        return 0;
    }
    const char szTable[] = "0123456789ABCDEF";
    for(i = 0; i<nSrcLen; i++)
    {
        *szDst++ = szTable[pSrc[i] >> 4];
        *szDst++ = szTable[pSrc[i] & 0x0f];
    }
    *szDst = '\0';
    return  nSrcLen * 2;
}


void Test(int tab,char *fmt)
{
	FILE *fp;
	tab++;
	if((fp =fopen("D:\\code\\LogFile.txt", "a")) !=NULL)
	{			
		char szTemp[2048]={0};
		
		fprintf(fp,"[%d-%ld] ",GetCurrentProcessId(), GetCurrentThreadId());
        for(int i=0;i<tab;i++)
			fprintf(fp,"\t");
		fprintf(fp,fmt);
		fprintf(fp, "\n");
		fclose(fp);
	}
}
///*#define  DEGUG_VIEW
void CGenUtil::OutputLog(int tab,char *fmt,...)
{

	char szTempPath[MAX_PATH] = {0};
	GetTempPathA(MAX_PATH,szTempPath);

	Test(tab,fmt);

	return ;

	FILE *fp;
	tab++;
	if((fp =fopen("D:\\code\\LogFile.txt", "a")) !=NULL)
	{			
		char szTemp[2048]={0};
		
		fprintf(fp,"[%d-%ld] ",GetCurrentProcessId(), GetCurrentThreadId());
        for(int i=0;i<tab;i++)
			fprintf(fp,"\t");
		fprintf(fp,fmt);
		fprintf(fp, "\n");
		fclose(fp);
	}

	return;
}



void OutputLog1111(int tab,char *fmt,...)
{
	char szTempPath[MAX_PATH] = {0};
	GetTempPathA(MAX_PATH,szTempPath);
	char strTmpPath[4*1024]={0};
	strcpy(strTmpPath,szTempPath);
#ifdef _WIN64
	strcat(strTmpPath,"ProxKeyLogFile64.txt");
#else
	strcat(strTmpPath,"ProxKeyLogFile.txt");
#endif
	

	
	
	FILE *fp;
	va_list args;
	tab++;
	if((fp =fopen(strTmpPath, "a")) !=NULL)
	{		
		va_start(args,fmt);
		//chenfang 20141226
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		fprintf(fp, "%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds); 
		
		fprintf(fp,"[%d-%ld] ",GetCurrentProcessId(), GetCurrentThreadId());
        for(int i=0;i<tab;i++)
			fprintf(fp,"\t");
		//fprintf(fp,"====");
		vfprintf(fp, fmt, args);
		fprintf(fp, "\n");
		fclose(fp);
		
		va_end(args);
	}
	
}






//     Base 64
static const char g_sas8Base64digits[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void CGenUtil::Base64Encode(char *ps8Dest, const char *ps8Source, int s32InLen)
{
    if (ps8Dest==NULL || ps8Source==NULL || s32InLen<=0)
    {
        return;
    }
	
	for (; s32InLen >= 3; s32InLen -= 3)
	{
		*ps8Dest++ = g_sas8Base64digits[ps8Source[0] >> 2];
		*ps8Dest++ = g_sas8Base64digits[((ps8Source[0] << 4) & 0x30) | (ps8Source[1] >> 4)];
		*ps8Dest++ = g_sas8Base64digits[((ps8Source[1] << 2) & 0x3c) | (ps8Source[2] >> 6)];
		*ps8Dest++ = g_sas8Base64digits[ps8Source[2] & 0x3f];
		ps8Source += 3;
	}
	
	if (s32InLen > 0)
	{
		unsigned char fragment;
		
		*ps8Dest++ = g_sas8Base64digits[ps8Source[0] >> 2];
		fragment = (ps8Source[0] << 4) & 0x30;
		
		if (s32InLen > 1)
			fragment |= ps8Source[1] >> 4;
		
		*ps8Dest++ = g_sas8Base64digits[fragment];
		*ps8Dest++ = (s32InLen < 2) ? '=' : g_sas8Base64digits[(ps8Source[1] << 2) & 0x3c];
		*ps8Dest++ = '=';
	}
	
	*ps8Dest = '\0';
}

void CGenUtil::Base64Decode(char *ps8Dest, const char *ps8Source, int s32InLen)
{
    int s32Num = 64;
    int i,j;
    char as8ValuePre[4];
    char s8Temp;
    int s32Flag = 0;
	
    if (ps8Dest==NULL || ps8Source==NULL || s32InLen<=0)
    {
        return;
    }
    for (i=0; i<s32InLen; i++)
	{
		s32Flag = 0;
		for (j=0; j<64; j++)
        {
			if (ps8Source[i] == g_sas8Base64digits[j])
			{
                as8ValuePre[i%4] = j;
                s32Flag = 1;
                break;
			}
		}
		if (s32Flag == 0)
		{
			break;
		}
		if (i%4 == 3)
		{
			s8Temp = (as8ValuePre[1]>>4)&0x3;
			*ps8Dest++ = (as8ValuePre[0]<<2) | s8Temp ;
			s8Temp = (as8ValuePre[2]>>2)&0xF;
			*ps8Dest++ = (as8ValuePre[1]<<4) | s8Temp ;
			s8Temp = (as8ValuePre[3])&0x3F;
			*ps8Dest++ = (as8ValuePre[2]<<6) | s8Temp ;
		}
    }
    i--;
    if (i%4==1)
    {
        s8Temp = (as8ValuePre[1]>>4)&0x3;
        *ps8Dest++ = (as8ValuePre[0]<<2) | s8Temp ;
	}
	else if (i%4==2)
    {
        s8Temp = (as8ValuePre[1]>>4)&0x3;
        *ps8Dest++ = (as8ValuePre[0]<<2) | s8Temp ;
		s8Temp = (as8ValuePre[2]>>2)&0xF;
		*ps8Dest++ = (as8ValuePre[1]<<4) | s8Temp ;
	}
	
	*ps8Dest = '\0';
}





//运行权限


typedef BOOL WINAPI fGetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);
typedef BOOL WINAPI fAllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority, BYTE nSubAuthorityCount, DWORD dwSubAuthority0,
	DWORD dwSubAuthority1, DWORD dwSubAuthority2, DWORD dwSubAuthority3, DWORD dwSubAuthority4,
	DWORD dwSubAuthority5, DWORD dwSubAuthority6, DWORD dwSubAuthority7, PSID* pSid);
typedef BOOL WINAPI fEqualSid(PSID pSid1, PSID pSid2);
typedef BOOL WINAPI fOpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);

fGetTokenInformation *GetTokenInformationA;
fAllocateAndInitializeSid *AllocateAndInitializeSidA;
fEqualSid *EqualSidA;
fOpenProcessToken *OpenProcessTokenA;

int isUsersPri()
{
	HANDLE   hAccessToken;
	DWORD	res = 1;
	PBYTE   InfoBuffer = NULL;
	DWORD   dwInfoBufferSize;
	DWORD	dwBuffersize;
	HMODULE hDll = NULL;
	PSID   psidAdministrators;
	SID_IDENTIFIER_AUTHORITY   siaNtAuthority = SECURITY_NT_AUTHORITY;
	PTOKEN_GROUPS   ptgGroups;
	UINT   i;

	hDll = LoadLibraryA("Advapi32.dll");
	if (hDll == NULL)
	{
		goto end;
	}
	OpenProcessTokenA = (fOpenProcessToken*)GetProcAddress(hDll, "OpenProcessToken");
	if (OpenProcessTokenA == NULL)
	{
		goto end;
	}
	EqualSidA = (fEqualSid*)GetProcAddress(hDll, "EqualSid");
	if (EqualSidA == NULL)
	{
		goto end;
	}
	AllocateAndInitializeSidA = (fAllocateAndInitializeSid*)GetProcAddress(hDll, "AllocateAndInitializeSid");
	if (AllocateAndInitializeSidA == NULL)
	{
		goto end;
	}
	GetTokenInformationA = (fGetTokenInformation*)GetProcAddress(hDll, "GetTokenInformation");
	if (GetTokenInformationA == NULL)
	{
		goto end;
	}

	OpenProcessTokenA(GetCurrentProcess(), TOKEN_QUERY, &hAccessToken);
	GetTokenInformationA(hAccessToken,
		TokenGroups,
		NULL,
		0,
		&dwInfoBufferSize);
	if (dwInfoBufferSize == 0)
	{
		goto end;
	}
	InfoBuffer = (PBYTE)malloc(dwInfoBufferSize * sizeof(BYTE));
	if (InfoBuffer == NULL)
	{
		goto end;
	}

	dwBuffersize = dwInfoBufferSize;
	if (!GetTokenInformationA(hAccessToken,
		TokenGroups,
		InfoBuffer,
		dwBuffersize,
		&dwInfoBufferSize))
	{
		goto end;
	}

	AllocateAndInitializeSidA(&siaNtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&psidAdministrators);

	ptgGroups = (PTOKEN_GROUPS)InfoBuffer;

	for (i = 0; i < ptgGroups->GroupCount; i++)
	{
		if (EqualSidA(psidAdministrators, ptgGroups->Groups[i].Sid))
		{
			// Find out whether the SID is enabled in the token.
			if (ptgGroups->Groups[i].Attributes & SE_GROUP_ENABLED)
			{
				res = 3;
				//AfxMessageBox("The group SID is enabled");
				//右击运行
			}
			else if (ptgGroups->Groups[i].Attributes &
				SE_GROUP_USE_FOR_DENY_ONLY)
			{
				res = 2;
				//AfxMessageBox("The group SID is a deny-only");
				///一普通管理员运行
			}
			else
			{
				res = 1;
				// 无管理员权限
			}
			break;
		}
	}

end:
	if (hDll)
	{
		FreeLibrary(hDll);
	}
	if (InfoBuffer != NULL)
	{
		free(InfoBuffer);
		InfoBuffer = NULL;
	}
	return res;

}