#pragma once
#include <string>
using namespace std;

#ifndef NOT_LENOVO_BUILD
#define LENOVO_SIGNER_NAME L"Lenovo"
#endif

//BOOL VerifyTrust(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks);
//BOOL CertGetName(LPCWSTR lpzFullModuleName, std::wstring* subject);
BOOL IsFileSignedBySubject(LPCWSTR lpzFullModuleName, LPCWSTR lpszSubject,
	BOOL WithoutRevocationChecks );

bool IsSignature(std::wstring dllPath, BOOL WithoutRevocationChecks = TRUE);