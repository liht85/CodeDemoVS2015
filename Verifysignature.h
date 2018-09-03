#pragma once
#include <string>
using namespace std;

//BOOL VerifyTrust(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks);
//BOOL CertGetName(LPCWSTR lpzFullModuleName, std::wstring* subject);
BOOL IsFileSignedBySubject(LPCWSTR lpzFullModuleName, LPCWSTR lpszSubject,
	BOOL WithoutRevocationChecks );

bool IsSignature(std::wstring dllPath, BOOL WithoutRevocationChecks = TRUE);