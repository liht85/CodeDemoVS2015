
#pragma once
#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include <string>



enum EnumLogLevel
{
	LogLevel_Stop = 0,
	LogLevel_Fatal,
	LogLevel_Error,
	LogLevel_Warning,
	LogLevel_Info
};

class CLogger
{
public:

	CLogger(EnumLogLevel nLogLevel = EnumLogLevel::LogLevel_Info, const std::string &strLogPath = "", const std::string &strLogName = "");
	virtual ~CLogger();
public:
	void TraceFatal(const char *lpcszFormat, ...);
	void TraceError(const char *lpcszFormat, ...);
	void TraceWarning(const char *lpcszFormat, ...);
	void TraceInfo(const char *lpcszFormat, ...);
	void ChangeLogLevel(EnumLogLevel nLevel);
	static std::string GetAppPathA();
	static std::string GetAppDataPath();
	static std::string FormatString(const char *lpcszFormat, ...);
private:
	void Trace(const std::string &strLog);
	std::string GetTime();
	const char *path_file(const char *path, char splitter);
private:
	FILE * m_pFileStream;
	EnumLogLevel m_nLogLevel;
	std::string m_strLogPath;
	std::string m_strLogName;
	std::string m_strLogFilePath;
	CRITICAL_SECTION m_cs;
};


#endif
