#pragma once
#pragma comment(lib, "ntdll.lib")


#include "bmp.h"


VOID EncryptBytesToBMP(PVOID pInBuffer, UINT iBufferLength, PBMP pBmp);
VOID DecryptBytesFromBMP(PVOID pOutBuffer, UINT iBufferLength, PBMP pBmp);

BOOL EmbedMessageInBMP(LPCWSTR lpszFilePath, PBMP pBmp);
BOOL RetrieveMessageFromBMP(PBMP pBmp, PVOID* pOutBuffer, PINT dwBytesReaded);