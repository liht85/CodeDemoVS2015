#ifndef _UPDATEPACKAGE_H_
#define _UPDATEPACKAGE_H_

typedef BOOL WINAPI fGetTokenInformation( HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength,PDWORD ReturnLength);
typedef BOOL WINAPI fAllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority, BYTE nSubAuthorityCount, DWORD dwSubAuthority0,
											  DWORD dwSubAuthority1, DWORD dwSubAuthority2,DWORD dwSubAuthority3, DWORD dwSubAuthority4,
											  DWORD dwSubAuthority5, DWORD dwSubAuthority6,DWORD dwSubAuthority7, PSID* pSid);
typedef BOOL WINAPI fEqualSid( PSID pSid1, PSID pSid2 );
typedef BOOL WINAPI fOpenProcessToken( HANDLE ProcessHandle,DWORD DesiredAccess,PHANDLE TokenHandle);
BOOL DownLoadSmallFile(CString pszUrlToDownload, CString pszFileToSave);
BOOL GetURLContext(CString strURL, CString strFilePath);
int	isUsersPri();
int CmpVersionNum(char *ver1, char *ver2);

#endif