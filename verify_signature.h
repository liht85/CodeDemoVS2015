/**
 * @file VerifySignature.h
 * @author keenwang@tencent.com
 * @date 2013/03/28
 * @version 1.0
 * @brief 提供验证签名的接口
 */
#ifndef VERIFY_SIGNATURE_H
#define VERIFY_SIGNATURE_H

#include <windows.h>
#include <string>

namespace Util {

//验证数字签名
BOOL IsFileSigned(LPCWSTR lpzFullModuleName, BOOL WithoutRevocationChecks = false);

// Check whether the file is signed by the |lpszSubject|
BOOL IsFileSignedBySubject(LPCWSTR lpszFullModuleName, LPCWSTR lpszSubject, 
    BOOL WithoutRevocationChecks = false);

//验证腾讯数字签名
// WithoutRevocationChecks is used to ensure the WinVerifyTrust function does not
// attempt any network retrieval when verifying code signatures.
//--bug=60589947, It may be failed to download CRL in some case.
BOOL IsFileSignedByTencent(LPCWSTR lpwszFilePath, BOOL WithoutRevocationChecks = false);

BOOL IsFileSignedByLenovo(LPCWSTR lpwszFilePath, BOOL WithoutRevocationChecks = false);

extern const WCHAR kAdobeSubjectName[];
} //Util

#endif //VERIFY_SIGNATURE_H