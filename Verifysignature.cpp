
#include "windows.h"
#include "Verifysignature.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include <wincrypt.h>
#pragma  comment(lib,"crypt32.lib") 

#include <wintrust.h>
#include <Softpub.h>
#pragma  comment(lib,"Wintrust.lib") 

#define LENOVO_SIGNER_NAME L"Lenovo"

BOOL VerifyTrust(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks) {
	if (!PathFileExists(lpzFullModuleName))
		return FALSE;
	
	LONG lStatus = TRUST_E_NOSIGNATURE;
	WINTRUST_FILE_INFO FileData;
	GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	WINTRUST_DATA WinTrustData;


	memset(&FileData, 0, sizeof(FileData));
	FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
	FileData.pcwszFilePath = lpzFullModuleName;
	FileData.hFile = NULL;
	FileData.pgKnownSubject = NULL;


	memset(&WinTrustData, 0, sizeof(WinTrustData));
	WinTrustData.cbStruct = sizeof(WinTrustData);

	// Use default code signing EKU.
	WinTrustData.pPolicyCallbackData = NULL;

	// Disable WVT UI.
	WinTrustData.dwUIChoice = WTD_UI_NONE;

	// No revocation checking.
	WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

	// Verify an embedded signature on a file.
	WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

	// Default verification.
	WinTrustData.dwStateAction = 0;

	// Not applicable for default verification of embedded signature.
	WinTrustData.hWVTStateData = NULL;

	// Not used.
	WinTrustData.pwszURLReference = NULL;

	// Default.
	WinTrustData.dwProvFlags = WTD_SAFER_FLAG;

	if (WithoutRevocationChecks)
		WinTrustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL;

	// This is not applicable if there is no UI because it changes 
	// the UI to accommodate running applications instead of 
	// installing applications.
	WinTrustData.dwUIContext = 0;

	// Set pFile.
	WinTrustData.pFile = &FileData;

	lStatus = WinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);

	return lStatus == ERROR_SUCCESS;
}




BOOL CertGetName(LPCWSTR lpzFullModuleName, std::wstring* subject) {

	BOOL bIsSuccess = FALSE;
	BOOL bRetCode = FALSE;
	DWORD dwRetCode = 0;

	HCERTSTORE hStore = NULL;
	HCRYPTMSG hMsg = NULL;
	DWORD dwEncoding = 0;
	DWORD dwContentType = 0;
	DWORD dwFormatType = 0;

	PCMSG_SIGNER_INFO pCMSGSignerInfo = NULL;
	PCCERT_CONTEXT pCertContext = NULL;
	CERT_INFO CertInfo = { 0 };
	DWORD dwInfoLen = 0;
	LPWSTR lpszSignerName = NULL;

	//Get Signature info
	bRetCode = CryptQueryObject(CERT_QUERY_OBJECT_FILE, lpzFullModuleName,
		CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
		CERT_QUERY_FORMAT_FLAG_BINARY,
		0, &dwEncoding, &dwContentType, &dwFormatType,
		&hStore, &hMsg, NULL);
	if (!bRetCode)
		goto End;

	// Get Signer information size.
	bRetCode = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL,
		&dwInfoLen);
	if (!bRetCode)
		goto End;

	// Allocate memory for Signer information.
	pCMSGSignerInfo = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwInfoLen);

	if (NULL == pCMSGSignerInfo)
		goto End;

	// Get Signer Information.
	bRetCode = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0,
		(PVOID)pCMSGSignerInfo, &dwInfoLen);
	if (!bRetCode)
		goto End;

	// Search for the signer certificate in the temporary 
	// certificate store.
	CertInfo.Issuer = pCMSGSignerInfo->Issuer;
	CertInfo.SerialNumber = pCMSGSignerInfo->SerialNumber;

	pCertContext = CertFindCertificateInStore(
		hStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_CERT,
		(PVOID)&CertInfo, NULL);
	if (NULL == pCertContext)
		goto End;

	//Get subject name size 
	dwInfoLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,
		0, NULL, NULL, 0);
	if (1 == dwInfoLen)
		goto End;

	// Get subject name.
	lpszSignerName = (WCHAR *)::LocalAlloc(LPTR, dwInfoLen * sizeof(WCHAR));
	dwRetCode = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,
		0, NULL, lpszSignerName, dwInfoLen);
	if (1 == dwRetCode)
		goto End;

	bIsSuccess = TRUE;
	if (subject)
		*subject = lpszSignerName;

End:
	if (lpszSignerName) {
		::LocalFree(lpszSignerName);
		lpszSignerName = NULL;
	}

	if (pCertContext) {
		CertFreeCertificateContext(pCertContext);
		pCertContext = NULL;
	}

	if (pCMSGSignerInfo) {
		::LocalFree(pCMSGSignerInfo);
		pCMSGSignerInfo = NULL;
	}

	if (hStore) {
		CertCloseStore(hStore, 0);
		hStore = NULL;
	}

	if (hMsg) {
		CryptMsgClose(hMsg);
		hMsg = NULL;
	}

	return bIsSuccess;
}


BOOL IsFileSignedBySubject(LPCWSTR lpzFullModuleName, LPCWSTR lpszSubject,
	BOOL WithoutRevocationChecks) {
	//检查是否有数字签名
	if (!VerifyTrust(lpzFullModuleName, WithoutRevocationChecks))
		return FALSE;

	std::wstring subject_name;
	if (!CertGetName(lpzFullModuleName, &subject_name))
		return FALSE;

	//compare
	return (0 == ::wcscmp(subject_name.c_str(), lpszSubject));
}

bool IsSignature(std::wstring dllPath, BOOL WithoutRevocationChecks )
{
	BOOL bRet;

	bRet = IsFileSignedBySubject(dllPath.c_str(), LENOVO_SIGNER_NAME, WithoutRevocationChecks);

	return (TRUE == bRet);
	
}