#include "stdafx.h"
#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <sys/timeb.h>
#include <vector>
#include <ImageHlp.h>

#pragma comment(lib,"imagehlp.lib")
#pragma comment( lib, "shell32.lib")
using std::string;
using std::vector;

CLogger::CLogger(EnumLogLevel nLogLevel, const std::string &strLogPath, const std::string &strLogName)
	:m_nLogLevel(nLogLevel),
	m_strLogPath(strLogPath),
	m_strLogName(strLogName)
{
	m_pFileStream = nullptr;
	if (m_strLogPath.empty())
	{
		m_strLogPath = GetAppDataPath();
		m_strLogPath.append("\\");
		m_strLogPath.append("Lenovo");
		m_strLogPath.append("\\");
		m_strLogPath.append("LenovoWrapper");
		m_strLogPath.append("\\");
	}
	if (m_strLogPath[m_strLogPath.length() - 1] != '\\')
	{
		m_strLogPath.append("\\");
	}
	MakeSureDirectoryPathExists(m_strLogPath.c_str());
	if (m_strLogName.empty())
	{
		time_t curTime;
		time(&curTime);
		tm tm1;
		localtime_s(&tm1, &curTime);
		//日志的名称如：201601012130.log
		m_strLogName = FormatString("%04d-%02d-%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday);
	}
	m_strLogFilePath = m_strLogPath.append(m_strLogName);

	fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");

	InitializeCriticalSection(&m_cs);
}


//析构函数
CLogger::~CLogger()
{
	//释放临界区
	DeleteCriticalSection(&m_cs);
	//关闭文件流
	if (m_pFileStream)
	{
		fclose(m_pFileStream);
		m_pFileStream = nullptr;
	}
}

//文件全路径得到文件名
const char *CLogger::path_file(const char *path, char splitter)
{
	return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
}

//写严重错误信息
void CLogger::TraceFatal(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别
	if (EnumLogLevel::LogLevel_Fatal > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	if (strResult.empty())
	{
		return;
	}
	string strLog = "[Fatal]\t";
	strLog.append(GetTime()).append("\t"+strResult);

	//写日志文件
	Trace(strLog);
}

//写错误信息
void CLogger::TraceError(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别
	if (EnumLogLevel::LogLevel_Error > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	if (strResult.empty())
	{
		return;
	}
	string strLog = "[Error]\t";
	strLog.append(GetTime()).append("\t"+strResult);

	//写日志文件
	Trace(strLog);
}

//写警告信息
void CLogger::TraceWarning(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别
	if (EnumLogLevel::LogLevel_Warning > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	if (strResult.empty())
	{
		return;
	}
	string strLog = "[Warning]\t";
	strLog.append(GetTime()).append("\t"+strResult);

	//写日志文件
	Trace(strLog);
}


//写一般信息
void CLogger::TraceInfo(const char *lpcszFormat, ...)
{
	//判断当前的写日志级别
	if (EnumLogLevel::LogLevel_Info > m_nLogLevel)
		return;
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	if (strResult.empty())
	{
		return;
	}
	string strLog = "[Info]\t";
	strLog.append(GetTime()).append("\t" + strResult);

	//写日志文件
	Trace(strLog);
}

//获取系统当前时间
string CLogger::GetTime()
{
	struct timeb tp_cur;
	ftime(&tp_cur);

	struct tm btm;

#ifdef WIN32
	localtime_s(&btm, &tp_cur.time);
#else
	localtime_r(&tp_cur.time, &btm);
#endif
	string strTime = FormatString("%04d-%02d-%02d %02d:%02d:%02d.%03d",
		btm.tm_year + 1900, btm.tm_mon + 1, btm.tm_mday,
		btm.tm_hour, btm.tm_min, btm.tm_sec, tp_cur.millitm);

	return strTime;
}

//改变写日志级别
void CLogger::ChangeLogLevel(EnumLogLevel nLevel)
{
	m_nLogLevel = nLevel;
}

//写文件操作
void CLogger::Trace(const string &strLog)
{
	try
	{
		//进入临界区
		EnterCriticalSection(&m_cs);
		//若文件流没有打开，则重新打开
		if (nullptr == m_pFileStream)
		{
			fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
			if (!m_pFileStream)
			{
				return;
			}
		}
		//写日志信息到文件流
		fprintf(m_pFileStream, "%s\n", strLog.c_str());
		fflush(m_pFileStream);
		//离开临界区
		LeaveCriticalSection(&m_cs);
		if (m_pFileStream)
		{
			fclose(m_pFileStream);
			m_pFileStream = nullptr;
		}
	}
	//若发生异常，则先离开临界区，防止死锁
	catch (...)
	{
		LeaveCriticalSection(&m_cs);
	}
}

string CLogger::GetAppPathA()
{
	char szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szFilePath, sizeof(szFilePath));
	_splitpath_s(szFilePath, szDrive, szDir, szFileName, szExt);

	string str(szDrive);
	str.append(szDir);
	return str;
}

std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char	*m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}

std::string CLogger::GetAppDataPath() {
	wchar_t m_lpszDefaultDir[MAX_PATH];
	wchar_t szDocument[MAX_PATH] = { 0 };
	memset(m_lpszDefaultDir, 0, _MAX_PATH);

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}	 
	return WcharToChar(m_lpszDefaultDir);
}

string CLogger::FormatString(const char *lpcszFormat, ...)
{
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	return strResult;
}

