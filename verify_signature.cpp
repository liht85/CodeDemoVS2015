#include "stdafx.h"

#include "verify_signature.h"

#include <WINCRYPT.H>
#include <WinTrust.h>
#include <Softpub.h>

namespace Util {

namespace {
//signer name 
#define TX_SIGNER_NAME L"Tencent Technology(Shenzhen) Company Limited"

#ifndef NOT_LENOVO_BUILD
#define LENOVO_SIGNER_NAME L"Lenovo"
#endif

//signer name 
#define TX_SIGNER_NAME L"Tencent Technology(Shenzhen) Company Limited"


  //dll
#define WINTRUST_DLL_NAME               L"Wintrust.dll"
#define CRYPT32_DLL_NAME                L"Crypt32.dll"

  //interface name
#define WINVERIFYTRUST                  ("WinVerifyTrust")
#define CRYPTQUERYOBJECT                ("CryptQueryObject")
#define CRYPTMSGGETPARAM                ("CryptMsgGetParam")
#define CRYPTDECODEOBJECT               ("CryptDecodeObject")
#define CERTFINDCERTIFICATEINSTORE      ("CertFindCertificateInStore")
#define CERTCREATECERTIFICATECONTEXT    ("CertCreateCertificateContext")
#define CERTFREECERTIFICATECONTEXT      ("CertFreeCertificateContext")
#define CRYPTMSGCLOSE                   ("CryptMsgClose")
#define CERTCLOSESTORE                  ("CertCloseStore")

#define CERTGETNAMESTRING               ("CertGetNameStringW")
#define fnCertGetNameString             fnCertGetNameStringW

  //function prototype
typedef  LONG(WINAPI * FPN_WinVerifyTrust)(
  __in    HWND hWnd,
  __in    GUID* pgActionID,
  __in    LPVOID pWVTData
  );

typedef BOOL(WINAPI *fnCryptQueryObject)
(
  __in  DWORD            dwObjectType,
  __in  const void       *pvObject,
  __in  DWORD            dwExpectedContentTypeFlags,
  __in  DWORD            dwExpectedFormatTypeFlags,
  __in  DWORD            dwFlags,
  __out  DWORD            *pdwMsgAndCertEncodingType,
  __out  DWORD            *pdwContentType,
  __out  DWORD            *pdwFormatType,
  __out  HCERTSTORE       *phCertStore,
  __out  HCRYPTMSG        *phMsg,
  __out  const void       **ppvContext
  );

typedef BOOL(WINAPI *fnCryptMsgGetParam)
(
  __in HCRYPTMSG hCryptMsg,
  __in DWORD dwParamType,
  __in DWORD dwIndex,
  __out void *pvData,
  __inout DWORD *pcbData
  );

typedef PCCERT_CONTEXT(WINAPI *fnCertFindCertificateInStore)
(
  __in HCERTSTORE hCertStore,
  __in DWORD dwCertEncodingType,
  __in DWORD dwFindFlags,
  __in DWORD dwFindType,
  __in const void *pvFindPara,
  __in PCCERT_CONTEXT pPrevCertContext
  );

typedef BOOL(WINAPI *fnCertFreeCertificateContext)
(
  __in PCCERT_CONTEXT pCertContext
  );

typedef BOOL(WINAPI *fnCryptMsgClose)
(
  __in HCRYPTMSG hCryptMsg
  );

typedef BOOL(WINAPI *fnCertCloseStore)
(
  __in HCERTSTORE hCertStore,
  __in DWORD dwFlags
  );

typedef DWORD(WINAPI *fnCertGetNameStringA)
(
  __in PCCERT_CONTEXT pCertContext,
  __in DWORD dwType,
  __in DWORD dwFlags,
  __in void *pvTypePara,
  __out LPSTR pszNameString,
  __in DWORD cchNameString
  );

typedef DWORD(WINAPI *fnCertGetNameStringW)
(
  __in PCCERT_CONTEXT pCertContext,
  __in DWORD dwType,
  __in DWORD dwFlags,
  __in void *pvTypePara,
  __out LPWSTR pszNameString,
  __in DWORD cchNameString
  );

BOOL VerifyTrust(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks = false);
BOOL VerifyTrust(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks/* = false*/) {
  base::FilePath file_path(lpzFullModuleName);
  if (!base::PathExists(file_path))
    return FALSE;

  HMODULE hWintrustDll = NULL;
  FPN_WinVerifyTrust fpnWinVerifyTrust = NULL;
  LONG lStatus = TRUST_E_NOSIGNATURE;
  WINTRUST_FILE_INFO FileData;
  GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;

  WINTRUST_DATA WinTrustData;

  base::FilePath strWinTrustPath;
  if (!PathService::Get(base::DIR_SYSTEM, &strWinTrustPath))
    return FALSE;

  strWinTrustPath = strWinTrustPath.Append(WINTRUST_DLL_NAME);
  hWintrustDll = ::LoadLibrary(strWinTrustPath.value().c_str());
  if (hWintrustDll == NULL) {
    lStatus = ::GetLastError();
    goto End;
  }

  fpnWinVerifyTrust = (FPN_WinVerifyTrust)::GetProcAddress(hWintrustDll,
                                                           "WinVerifyTrust");
  if (fpnWinVerifyTrust == NULL) {
    lStatus = ::GetLastError();
    goto End;
  }

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
    WinTrustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL ;

  // This is not applicable if there is no UI because it changes 
  // the UI to accommodate running applications instead of 
  // installing applications.
  WinTrustData.dwUIContext = 0;

  // Set pFile.
  WinTrustData.pFile = &FileData;

  lStatus = fpnWinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);

End:
  if (hWintrustDll != NULL) {
    ::FreeLibrary(hWintrustDll);
  }

  return lStatus == ERROR_SUCCESS;
}

BOOL CertGetName(LPCWSTR lpzFullModuleName, std::wstring* subject) {
  //init module
  base::FilePath strCrypt32Path;
  if (!PathService::Get(base::DIR_SYSTEM, &strCrypt32Path))
    return FALSE;
  strCrypt32Path = strCrypt32Path.Append(CRYPT32_DLL_NAME);

  HMODULE hModCrypt32Dll = ::LoadLibrary(strCrypt32Path.value().c_str());
  if (hModCrypt32Dll == NULL)
    return  FALSE;

  fnCryptQueryObject pfnCryptQueryObject =
    (fnCryptQueryObject)::GetProcAddress(hModCrypt32Dll, CRYPTQUERYOBJECT);
  fnCryptMsgGetParam pfnCryptMsgGetParam =
    (fnCryptMsgGetParam)::GetProcAddress(hModCrypt32Dll, CRYPTMSGGETPARAM);
  fnCryptMsgClose pfnCryptMsgClose =
    (fnCryptMsgClose)::GetProcAddress(hModCrypt32Dll, CRYPTMSGCLOSE);
  fnCertFindCertificateInStore pfnCertFindCertificateInStore =
    (fnCertFindCertificateInStore)::GetProcAddress(hModCrypt32Dll,
                                                   CERTFINDCERTIFICATEINSTORE);
  fnCertFreeCertificateContext pfnCertFreeCertificateContext =
    (fnCertFreeCertificateContext)::GetProcAddress(hModCrypt32Dll,
                                                   CERTFREECERTIFICATECONTEXT);
  fnCertCloseStore pfnCertCloseStore =
    (fnCertCloseStore)::GetProcAddress(hModCrypt32Dll, CERTCLOSESTORE);
  fnCertGetNameString pfnCertGetNameString =
    (fnCertGetNameString)::GetProcAddress(hModCrypt32Dll, CERTGETNAMESTRING);

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
  bRetCode = pfnCryptQueryObject(CERT_QUERY_OBJECT_FILE, lpzFullModuleName,
                                 CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                                 CERT_QUERY_FORMAT_FLAG_BINARY,
                                 0, &dwEncoding, &dwContentType, &dwFormatType,
                                 &hStore, &hMsg, NULL);
  if (!bRetCode)
    goto End;

  // Get Signer information size.
  bRetCode = pfnCryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, NULL,
                                 &dwInfoLen);
  if (!bRetCode)
    goto End;

  // Allocate memory for Signer information.
  pCMSGSignerInfo = (PCMSG_SIGNER_INFO)::LocalAlloc(LPTR, dwInfoLen);

  if (NULL == pCMSGSignerInfo)
    goto End;

  // Get Signer Information.
  bRetCode = pfnCryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0,
                                 (PVOID)pCMSGSignerInfo, &dwInfoLen);
  if (!bRetCode)
    goto End;

  // Search for the signer certificate in the temporary 
  // certificate store.
  CertInfo.Issuer = pCMSGSignerInfo->Issuer;
  CertInfo.SerialNumber = pCMSGSignerInfo->SerialNumber;

  pCertContext = pfnCertFindCertificateInStore(
    hStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0, CERT_FIND_SUBJECT_CERT,
    (PVOID)&CertInfo, NULL);
  if (NULL == pCertContext)
    goto End;

  //Get subject name size 
  dwInfoLen = pfnCertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                   0, NULL, NULL, 0);
  if (1 == dwInfoLen)
    goto End;

  // Get subject name.
  lpszSignerName = (WCHAR *)::LocalAlloc(LPTR, dwInfoLen * sizeof(WCHAR));
  dwRetCode = pfnCertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,
                                   0, NULL, lpszSignerName, dwInfoLen);
  if (1 == dwRetCode)
    goto End;

  bIsSuccess = TRUE;
  if (subject)
    *subject = lpszSignerName;

End:
  //uninit
  if (lpszSignerName) {
    ::LocalFree(lpszSignerName);
    lpszSignerName = NULL;
  }

  if (pCertContext) {
    pfnCertFreeCertificateContext(pCertContext);
    pCertContext = NULL;
  }

  if (pCMSGSignerInfo) {
    ::LocalFree(pCMSGSignerInfo);
    pCMSGSignerInfo = NULL;
  }

  if (hStore) {
    pfnCertCloseStore(hStore, 0);
    hStore = NULL;
  }

  if (hMsg) {
    pfnCryptMsgClose(hMsg);
    hMsg = NULL;
  }

  if (hModCrypt32Dll) {
    ::FreeLibrary(hModCrypt32Dll);
    hModCrypt32Dll = NULL;
  }

  return bIsSuccess;
}

} // namespace

const WCHAR kAdobeSubjectName[] = L"Adobe Systems Incorporated";

//impl
BOOL IsFileSigned(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks/* = false*/) {
  if (nullptr == lpzFullModuleName)
    return FALSE;

  return VerifyTrust(lpzFullModuleName, WithoutRevocationChecks);
}

BOOL IsFileSignedBySubject(LPCWSTR lpzFullModuleName, LPCWSTR lpszSubject, 
    BOOL WithoutRevocationChecks/* = false*/) {
  //检查是否有数字签名
  if (!IsFileSigned(lpzFullModuleName, WithoutRevocationChecks))
    return FALSE;

  std::wstring subject_name;
  if (!CertGetName(lpzFullModuleName, &subject_name))
    return FALSE;

  //compare
  return (0 == ::wcscmp(subject_name.c_str(), lpszSubject));
}

BOOL IsFileSignedByTencent(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks/* = false*/) {
  return IsFileSignedBySubject(lpzFullModuleName, TX_SIGNER_NAME,
      WithoutRevocationChecks);
}

BOOL IsFileSignedByLenovo(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks) {
  return IsFileSignedBySubject(lpzFullModuleName, LENOVO_SIGNER_NAME,
      WithoutRevocationChecks);
}

} // namespace util
