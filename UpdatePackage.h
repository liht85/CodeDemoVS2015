#ifndef _UPDATEPACKAGE_H_
#define _UPDATEPACKAGE_H_


BOOL DownLoadSmallFile(CString pszUrlToDownload, CString pszFileToSave);
BOOL GetURLContext(CString strURL, CString strFilePath);
int	isUsersPri();
int CmpVersionNum(char *ver1, char *ver2);

#endif