#include "stdafx.h"
#include <time.h>
#include <stdarg.h>
#include <vector>

#include "outputLogToFile.h"
#include "imagehlp.h"
#pragma comment(lib,"imagehlp.lib")
#include <string.h>

BOOL  GetAppDataPath(char *pszDefaultDir) {

	char szDocument[MAX_PATH] = { 0 };
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDListA(pidl, szDocument))
	{
		return (0 != GetShortPathNameA(szDocument, pszDefaultDir, _MAX_PATH));
	}
	return FALSE;
}

void debugInfo(char *pFolderName, char *pFile, int iLineNumb, bool bTAG, char *fmt, ...)
{
	BOOL bRet;
	string strLog;
	tm timeInfo;
	time_t curTime;
	char szFileName[30] = { 0 };
	char szFilePath[MAX_PATH] = { 0 };
	time(&curTime);
	localtime_s(&timeInfo, &curTime);
	bRet = GetAppDataPath(szFilePath);
	if (!bRet)
	{
		return;
	}
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), "\\");
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), pFolderName);
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), "\\");
	bRet = MakeSureDirectoryPathExists(szFilePath);	//  文件夹不存在 则创建  路径格式必须 以 \\  结尾 如：“  d:\\ddd1\\log\\ ”   
	if (!bRet)
	{
		return;
	}
	snprintf(szFileName, sizeof(szFileName), "%d%02d%02d.log", timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), szFileName);
	FILE *pLogHandle = nullptr;
	fopen_s(&pLogHandle, szFilePath, "a+");
	if (NULL == pLogHandle)
		return;
	char szHead[_MAX_PATH] = { 0 };
	if (bTAG)
	{
		snprintf(szHead, sizeof(szHead), "[INF] [%d%02d%02d/%02d:%02d:%02d][%-25s- %04d]",
			timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
			timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, pFile, iLineNumb);
	}
	else
	{
		snprintf(szHead, sizeof(szHead), "[ERR] [%d%02d%02d/%02d:%02d:%02d][%-25s- %04d]",
			timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
			timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, pFile, iLineNumb);
	}

	int iLen = strlen(szHead);
	fwrite(szHead, iLen, 1, pLogHandle);
	fwrite("\t", 1, 1, pLogHandle);
	va_list args;
	va_start(args, fmt);
	size_t nLength = _vscprintf(fmt, args) + 2;  // 获取格式化字符串长度
	std::vector<char> vBuffer(nLength, '\0');
	int nWritten = vsnprintf(&vBuffer[0], vBuffer.size(), fmt, args);
	va_end(args);
	if (nWritten > 0)
	{
		strLog = &vBuffer[0];
	}
	fwrite(strLog.c_str(), strLog.length(), 1, pLogHandle);
	fwrite("\n", 1, 1, pLogHandle);
	fclose(pLogHandle);
	return;
}




MyLog::MyLog()
{
	InitializeCriticalSection(&m_cs);
	m_hMutex = CreateMutexA(NULL, FALSE, strMutexName.c_str());

	m_strFolderName = "liht_LogFloder";

	time(&curTime);
	localtime_s(&timeInfo, &curTime);
	BOOL bRet = GetAppDataPath(szFilePath);
	if (!bRet)
	{
		MessageBoxA(NULL, "1", "1", NULL);
	}
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), "\\");
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), m_strFolderName.c_str());
	strcat_s(szFilePath, sizeof(szFilePath) / sizeof(szFilePath[0]), "\\");
	snprintf(szFileName, sizeof(szFileName), "%s%d%02d%02d.log", szFilePath, timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);

	bRet = MakeSureDirectoryPathExists(szFilePath);	//  文件夹不存在 则创建  路径格式必须 以 \\  结尾 如：“  d:\\ddd1\\log\\ ”   
	if (!bRet)
	{
		MessageBoxA(NULL, "2", "2", NULL);
	}

}
MyLog::~MyLog()
{
	DeleteCriticalSection(&m_cs);
}

void MyLog::StartMutex(HANDLE hMutex)
{
	if (hMutex != NULL)
	{
		WaitForSingleObject(hMutex, INFINITE);
	}
}
void MyLog::StopMutex(HANDLE hMutex)
{
	if (hMutex != NULL)
	{
		ReleaseMutex(hMutex);
	}
}

void MyLog::traceEX(BOOL bTAG, char *pFile, int iLineNumb, char *fmt, ...)
{
 	EnterCriticalSection(&m_cs);
	StartMutex(m_hMutex);
	string strLog;
	int nWritten = 0;	
	char szHead[_MAX_PATH] = { 0 };
	size_t nLength = 0;
	std::vector<char> vBuffer;
	if (bTAG)
	{
		snprintf(szHead, sizeof(szHead), "[INF] [%d-%d] [%d%02d%02d/%02d:%02d:%02d][%-25s- %04d]", GetCurrentProcessId(), GetCurrentThreadId(),
			timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
			timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, pFile, iLineNumb);
	}
	else
	{
		snprintf(szHead, sizeof(szHead), "[ERR] [%d-%d] [%d%02d%02d/%02d:%02d:%02d][%-25s- %04d]", GetCurrentProcessId(), GetCurrentThreadId(),
			timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
			timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, pFile, iLineNumb);
	}

	if (nullptr == pLogHandle)
	{
		int i = 0;
		fopen_s(&pLogHandle, szFileName, "a+");
		while  (nullptr == pLogHandle)
		{
			if (i>10)
			{
				goto end;
			}
			fopen_s(&pLogHandle, szFileName, "a+");
			Sleep(50);
			i++;
		}
	}

	fwrite(szHead, strlen(szHead), 1, pLogHandle);
	fwrite("\t", 1, 1, pLogHandle);
	va_list args;
	va_start(args, fmt);

	nLength = _vscprintf(fmt, args) + 2;  // 获取格式化字符串长度
	vBuffer.resize(nLength, '\0');
	nWritten = vsnprintf(&vBuffer[0], vBuffer.size(), fmt, args);
	va_end(args);
	if (nWritten > 0)
	{
		strLog = &vBuffer[0];
	}

	fwrite(strLog.c_str(), strLog.length(), 1, pLogHandle);
	fwrite("\n", 1, 1, pLogHandle);
	fclose(pLogHandle);
	pLogHandle = nullptr;
end:
 	LeaveCriticalSection(&m_cs);
 	StopMutex(m_hMutex);
	return;
}