#pragma once
#include <string>
using std::string;

#define LOG_FILE_PATH          "log"   
#define LOG_PARAMS             LOG_FILE_PATH,__FILE__,__LINE__ 

#define LOGSTRINGS(fm, ...) printf(fm,__VA_ARGS__)


#define DEBUGINFRERR(fm, ...)	debugInfo(LOG_PARAMS,false,fm,__VA_ARGS__)
#define DEBUGINFRINF(fm, ...)	debugInfo(LOG_PARAMS,true,fm,__VA_ARGS__)
void debugInfo(char *pFolderName, char *pFile, int iLineNumb, bool bTAG,char *fmt, ...);

class MyLog
{
public:
	MyLog();
	~MyLog();
	void traceEX(BOOL bTAG, char *pFile, int iLineNumb, char *fmt, ...);
private:
	char szFilePath[MAX_PATH] = { 0 };
	char szFileName[_MAX_PATH] = { 0 };
	FILE *pLogHandle = nullptr;
	tm timeInfo;
	time_t curTime;
	string m_strFolderName;
	CRITICAL_SECTION m_cs;
	HANDLE m_hMutex = NULL;
	string strMutexName = "Mutex_Name";
	void StartMutex(HANDLE hMutex);
	void StopMutex(HANDLE hMutex);
	
};