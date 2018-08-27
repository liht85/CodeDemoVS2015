#include "stdafx.h"
#include <afxinet.h>
#include "UpdatePackage.h"

/*
将原来的比较版本号字符串改为比较版本号数值
解决1.0.0.9 > 1.0.0.10 的问题。
*/
#define VERSION_INFO(w,x,y,z) ((w<<24) | (x<<16) | (y<<8) | (z))
int CmpVersionNum(char *ver1, char *ver2)
{
	int  iRet = 0;
	int  iCmpRst = 0;

	int iVer1[4] = { 0 },
		iVer2[4] = { 0 };

	iRet = sscanf_s(ver1, "%d.%d.%d.%d", &iVer1[0], &iVer1[1], &iVer1[2], &iVer1[3]);
	if (-1 == iRet)
	{
		return -3;
	}
	iRet = sscanf_s(ver2, "%d.%d.%d.%d", &iVer2[0], &iVer2[1], &iVer2[2], &iVer2[3]);
	if (-1 == iRet)
	{
		return -3;
	}

	iCmpRst = VERSION_INFO(iVer1[0], iVer1[1], iVer1[2], iVer1[3])
		- VERSION_INFO(iVer2[0], iVer2[1], iVer2[2], iVer2[3]);

	if (iCmpRst > 0)
	{
		iRet = 1;
	}
	else if (iCmpRst < 0)
	{
		iRet = -1;
	}
	else
	{
		return 0;
	}

	return iRet;
}

#define SECURITY_IGNORE_ERROR_MASK  (INTERNET_FLAG_IGNORE_CERT_CN_INVALID   |  \
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  \
	SECURITY_FLAG_IGNORE_UNKNOWN_CA        |  \
                                     SECURITY_FLAG_IGNORE_REVOCATION    )

BOOL DownLoadSmallFile(CString pszUrlToDownload, CString pszFileToSave)
{
	INTERNET_PORT wPort;
	CString strServer, strObject;
	DWORD dwServiceType;

	//Now do the actual read of the file
	DWORD dwStartTicks = ::GetTickCount();
	DWORD dwCurrentTicks = dwStartTicks;
	DWORD dwBytesRead = 0;
	char szReadBuf[1024];
	DWORD dwBytesToRead = 1024;
	DWORD dwTotalBytesRead = 0;
	DWORD dwLastTotalBytes = 0;
	DWORD dwLastPercentage = 0;
	// Get the length of the file.            
	TCHAR szContentLength[32];
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = 32;

	DWORD dwFileSize = 0;
	BOOL bGotFileSize = FALSE;

	BOOL bSend = FALSE;
	int retrydigua = 0;

	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;

	DWORD dwFlag = 0;
	//MessageBox(NULL, "DownLoadSmallFile coming", 0, 0);
	if (!AfxParseURL(pszUrlToDownload, dwServiceType, strServer, strObject, wPort))
	{
		//Try sticking "http://" before it
		CString url;
		url = pszUrlToDownload;
		url += _T("http://");
		url += pszUrlToDownload;
		if (!AfxParseURL(url, dwServiceType, strServer, strObject, wPort))
		{
			return FALSE;
		}
	}

	CFile fileToWrite;
	//Try and open the file we will download into
	if (!fileToWrite.Open(pszFileToSave, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
	{
		TRACE(_T("Failed to open the file to download into, Error:%d\n"), GetLastError());
		CString sError;
		sError.Format(_T("%d"), ::GetLastError());
		CString sMsg;
		return FALSE;
	}

	//Pull out just the filename component
	CString sFilename;
	int nSlash = strObject.ReverseFind(_T('/'));
	if (nSlash == -1)
		nSlash = strObject.ReverseFind(_T('\\'));
	if (nSlash != -1 && strObject.GetLength() > 1)
		sFilename = strObject.Right(strObject.GetLength() - nSlash - 1);
	else
		sFilename = strObject;


	HINTERNET hInternetSession = NULL;
	hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		TRACE(_T("Failed in call to InternetOpen, Error:%d\n"), ::GetLastError());
		return FALSE;
	}

	HINTERNET     hHttpConnection = NULL;
	HINTERNET     hHttpFile = NULL;
	hHttpConnection = ::InternetConnect(hInternetSession, strServer, /*INTERNET_DEFAULT_HTTPS_PORT,*/ wPort, NULL,
		NULL, INTERNET_SERVICE_HTTP /*m_dwServiceType*/, 0, NULL);

	BOOL bRet = FALSE;
	if (hHttpConnection == NULL)
	{
		TRACE(_T("Failed in call to InternetConnect, Error:%d\n"), ::GetLastError());
		goto goout;
	}



	// add by digua for download file from https site
	if (dwServiceType == AFX_INET_SERVICE_HTTPS)
	{
		dwFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_SECURE | SECURITY_IGNORE_ERROR_MASK;
	}
	else
	{
		dwFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
	}
	hHttpFile = HttpOpenRequest(hHttpConnection, NULL, strObject, NULL, NULL, ppszAcceptTypes, dwFlag, NULL);
	if (hHttpFile == NULL)
	{
		TRACE(_T("Failed in call to HttpOpenRequest, Error:%d\n"), ::GetLastError());
		goto goout;
	}

resend:

	retrydigua++;
	//Issue the request
	bSend = ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0);
	if (!bSend)
	{
		DWORD dwError = GetLastError();
		if (2 > retrydigua)
		{
			DWORD dwFlags;
			DWORD dwBuffLen = sizeof(dwFlags);

			InternetQueryOption(hHttpFile, INTERNET_OPTION_SECURITY_FLAGS,
				(LPVOID)&dwFlags, &dwBuffLen);

			switch (dwError)
			{
			case ERROR_INTERNET_INVALID_CA:
			{
				dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			}
			break;
			case ERROR_INTERNET_SEC_CERT_REV_FAILED:
			{
				dwFlags |= SECURITY_FLAG_IGNORE_REVOCATION;
			}
			break;
			default:
			{
				dwFlags |= SECURITY_IGNORE_ERROR_MASK;
			}
			break;
			}
			InternetSetOption(hHttpFile, INTERNET_OPTION_SECURITY_FLAGS,
				&dwFlags, sizeof(dwFlags));
			goto resend;
		}
		else if (retrydigua < 1000)
		{
			// Make sure to check return code from InternetErrorDlg
			// user may click either OK or Cancel. In case of Cancel
			// request should not be resumbitted.
			DWORD dwRet = 0;
			dwRet = InternetErrorDlg(::GetDesktopWindow(),
				hHttpFile,
				dwError,
				FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |
				FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
				NULL);
			retrydigua = 1000;
			goto resend;
		}
		TRACE(_T("Failed in call to HttpSendRequest, Error:%d\n"), ::GetLastError());
		goto goout;
	}

	//Check the HTTP status code

	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		TRACE(_T("Failed in call to HttpQueryInfo for HTTP query status code, Error:%d\n"), ::GetLastError());
		goto goout;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);

		//Handle any authentication errors
		if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[51];
			DWORD dwSize;
			do
			{
				::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
			} while (dwSize != 0);

			//Bring up the standard authentication dialog
			if (::InternetErrorDlg(NULL, hHttpFile, ERROR_INTERNET_INCORRECT_PASSWORD, FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL) == ERROR_INTERNET_FORCE_RETRY)
				goto resend;
		}
		else if (nStatusCode != HTTP_STATUS_OK)
		{
			TRACE(_T("Failed to retrieve a HTTP 200 status, Status Code:%d\n"), nStatusCode);
			goto goout;
		}
	}


	if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		//Set the progress control range
		bGotFileSize = TRUE;
		dwFileSize = (DWORD)_ttol(szContentLength);
	}


	do
	{
		if (!::InternetReadFile(hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead))
		{
			TRACE(_T("Failed in call to InternetReadFile, Error:%d\n"), ::GetLastError());
			goto goout;
		}
		else if (dwBytesRead)
		{
			//Write the data to file
			TRY
			{
				fileToWrite.Write(szReadBuf, dwBytesRead);
			}
			CATCH(CFileException, e);
			{
				TRACE(_T("An exception occured while writing to the download file\n"));
				e->Delete();
				goto goout;
			}
			END_CATCH

				//Increment the total number of bytes read
				dwTotalBytesRead += dwBytesRead;
		}
	} while (dwBytesRead);

	//Delete the file being downloaded to if it is present and the download was aborted
	fileToWrite.Close();
	bRet = TRUE;
goout:
	//Free up the internet handles we may be using
	if (hHttpFile)
	{
		::InternetCloseHandle(hHttpFile);
		hHttpFile = NULL;
	}
	if (hHttpConnection)
	{
		::InternetCloseHandle(hHttpConnection);
		hHttpConnection = NULL;
	}
	if (hInternetSession)
	{
		::InternetCloseHandle(hInternetSession);
		hInternetSession = NULL;
	}

	return bRet;
}

BOOL GetURLContext(CString strURL, CString strFilePath)
{
	CInternetSession session; //»á»°ÆÚ¶ÔÏó)
	CHttpConnection* pServer = NULL; // Ö¸Ïò·þÎñÆ÷µØÖ·(URL)
	CHttpFile * pHttpFile = NULL;//HTTPÎÄ¼þÖ¸Õë
	CString strServerName; //·þÎñÆ÷Ãû
	CString strObject; //²éÑ¯¶ÔÏóÃû(httpÎÄ¼þ)
	INTERNET_PORT nPort; //¶Ë¿Ú
	DWORD dwServiceType; //·þÎñÀàÐÍ
	DWORD dwHttpRequestFlags = //ÇëÇó±êÖ¾
		//INTERNET_FLAG_EXISTING_CONNECT;
		INTERNET_FLAG_RELOAD;
	const TCHAR szHeaders[] = _T("Accept: */*\r\nUser-Agent:HttpClient\r\n");
	TCHAR szBuf[1024] = { 0 }; //»º´æ
	HANDLE hFile = NULL;
	int length = 0;
	DWORD dwLenWritten = 0;
	LONGLONG llCount = 0;
	CString str;

	BOOL OK = AfxParseURL(strURL, //±»·ÖÎöURL´®
		dwServiceType, //·þÎñÀàÐÍ£¬ftp£¬httpµÈ
		strServerName, //·þÎñÆ÷Ãû
		strObject, //URLÖÐ±»²éÑ¯¶ÔÏó
		nPort); //URLÖ¸¶¨µÄ¶Ë¿Ú£¬¿ÉÄÜÎª¿Õ
	OK = OK && (dwServiceType == INTERNET_SERVICE_HTTP);//±¾ÀýÖ»¿¼ÂÇhttpÐ­Òé
	if (!OK)
	{
		return FALSE;
	}

	hFile = CreateFile(strFilePath, GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	try
	{
		pServer = session.GetHttpConnection(strServerName, nPort); //»ñµÃ·þÎñÆ÷Ãû
		pHttpFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		//Ïò·þÎñÆ÷·¢ËÍÇëÇó£¬½¨Á¢httpÁ¬½Ó£¬
		//½¨Á¢±¾»úÉÏµÄhttpÎÄ¼þÖ¸Õë
		pHttpFile->AddRequestHeaders(szHeaders);
		pHttpFile->SendRequest(); //·¢ËÍÇëÇó
		while ((length = pHttpFile->Read(szBuf, 1023)) != 0)
		{

			WriteFile(hFile, szBuf, length, &dwLenWritten, NULL);

			llCount++;
		}
		CloseHandle(hFile);
	}
	catch (CInternetException*)
	{
		pHttpFile->Close();
		pServer->Close();
		if (pHttpFile != NULL)
			delete pHttpFile;
		if (pServer != NULL)
			delete pServer;
		CloseHandle(hFile);
		return FALSE;
	}

	pHttpFile->Close();
	pServer->Close();
	if (pHttpFile != NULL)
		delete pHttpFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	return TRUE;
}