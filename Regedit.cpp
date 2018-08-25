#include "stdafx.h"
#include "Regedit.h"
wstring	GetBrowserRegitValueByKey(LPCWSTR szKey)
{
	wstring strRet = L"";

	WCHAR szwAppPath[MAX_PATH] = { 0 };
	DWORD cbData = MAX_PATH;
	DWORD dwDataTypeSZ = REG_SZ;
	ZeroMemory(szwAppPath, sizeof(szwAppPath));
	HKEY hk = NULL;
//	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Lenovo\\SLBrowser", 0, KEY_READ | KEY_QUERY_VALUE, &hk) != ERROR_SUCCESS) {

		if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lenovo\\SLBrowser", 0, KEY_READ | KEY_QUERY_VALUE, &hk) != ERROR_SUCCESS) {

		}
//	}
	else
	{
		if (::RegQueryValueEx(hk, szKey, NULL, &dwDataTypeSZ, (LPBYTE)szwAppPath, &cbData) == ERROR_SUCCESS)
		{
			strRet = szwAppPath;
		}
		else
		{
			//PrintLog(L"获取值失败");
		}
	}

	::RegCloseKey(hk);

	return strRet;
}



DWORD	GetBrowserRegitValueByKeyDWOD(LPCWSTR szKey)
{
	DWORD strRet = 0;

	DWORD szwAppPath =  0 ;
	DWORD cbData = MAX_PATH;
	DWORD dwDataTypeSZ = REG_DWORD;

	HKEY hk = NULL;
	//if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Lenovo\\SLBrowser", 0, KEY_READ | KEY_QUERY_VALUE, &hk) != ERROR_SUCCESS) {

		if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Lenovo\\SLBrowser", 0, KEY_READ | KEY_QUERY_VALUE, &hk) != ERROR_SUCCESS) {

		}
//	}
	else
	{
		if (::RegQueryValueEx(hk, szKey, NULL, &dwDataTypeSZ, (LPBYTE)&szwAppPath, &cbData) == ERROR_SUCCESS)
		{
			strRet = szwAppPath;
		}
		else
		{
			//PrintLog(L"获取值失败");
		}
	}

	::RegCloseKey(hk);

	return strRet;
}
