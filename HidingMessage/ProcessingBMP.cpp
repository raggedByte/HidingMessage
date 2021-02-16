#include "ProcessingBMP.h"


VOID EncryptBytesToBMP(PVOID pInBuffer, UINT iBufferLength, PBMP pBmp)
{
	PBYTE pBuffer = (PBYTE)pInBuffer;
	for (UINT k = 0; k < iBufferLength; k++)
	{
		for (INT i = 0; i < 8; i++)
		{
			*(pBmp->pWriter) = ((*pBuffer >> i) & 1) ? *(pBmp->pWriter) | 1 : *(pBmp->pWriter) & 254;
			pBmp->pWriter++;
		}
		pBuffer++;
	}
}
VOID DecryptBytesFromBMP(PVOID pOutBuffer, UINT iBufferLength, PBMP pBmp)
{
	PBYTE pBuffer = (PBYTE)pOutBuffer;
	for (UINT k = 0; k < iBufferLength; k++)
	{
		for (INT i = 0; i < 8; i++)
		{
			*pBuffer = (*(pBmp->pReader) & 1) ? *pBuffer | (1 << i) : *pBuffer & ~(1 << i);
			pBmp->pReader++;
		}
		pBuffer++;
	}
}
BOOL EmbedMessageInBMP(LPCWSTR lpszFilePath, PBMP pBmp)
{
	HANDLE hFile;
	DWORD dwReadedByte;
	DWORD dwCRC;
	LONG biWidthBMP, biHeightBMP;
	PBYTE pBuffer;

	hFile = CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	DWORD dwSizeH = 0, dwSizeL = 0;

	dwSizeL = GetFileSize(hFile, &dwSizeH);

	LONGLONG fileLength = ((LONGLONG)dwSizeH * ((LONGLONG)MAXDWORD + 1)) + dwSizeL;

	if (fileLength >= MAXDWORD)
	{
		return FALSE;
	}

	biWidthBMP = pBmp->infoHeader.biWidth;
	biHeightBMP = pBmp->infoHeader.biHeight;

	if (biWidthBMP * biHeightBMP * 3 / 8 < fileLength + sizeof(DWORD) + sizeof(DWORD))
	{
		return FALSE;
	}

	pBuffer = (PBYTE)malloc(sizeof(BYTE) * fileLength);

	if (pBuffer == NULL)
	{
		return FALSE;
	}

	if (!ReadFile(hFile, pBuffer, (DWORD)fileLength, &dwReadedByte, NULL))
	{
		return FALSE;
	}

	if (dwReadedByte == 0)
	{
		return FALSE;
	}

	dwCRC = RtlCrc32(pBuffer, fileLength, 0);

	EncryptBytesToBMP(&dwCRC, sizeof(DWORD), pBmp);
	EncryptBytesToBMP(&fileLength, sizeof(DWORD), pBmp);
	EncryptBytesToBMP(pBuffer, (DWORD)fileLength, pBmp);

	CloseHandle(hFile);

	return TRUE;
}
BOOL RetrieveMessageFromBMP(PBMP pBmp, PVOID* pOutBuffer, PINT dwBytesReaded)
{
	DWORD dwCRC, dwCheckCRC;
	LONG biWidthBMP, biHeightBMP;
	DWORD dwReadedBytes;
	PBYTE pBuffer;

	DecryptBytesFromBMP(&dwCRC, sizeof(DWORD), pBmp);
	DecryptBytesFromBMP(&dwReadedBytes, sizeof(DWORD), pBmp);

	biWidthBMP = pBmp->infoHeader.biWidth;
	biHeightBMP = pBmp->infoHeader.biHeight;

	if (biWidthBMP * biHeightBMP * 3 / 8 < dwReadedBytes + sizeof(DWORD) + sizeof(DWORD))
	{
		return FALSE;
	}

	pBuffer = (PBYTE)malloc(sizeof(BYTE) * dwReadedBytes);

	if (pBuffer == NULL)
	{
		return FALSE;
	}

	DecryptBytesFromBMP(pBuffer, (DWORD)dwReadedBytes, pBmp);

	dwCheckCRC = RtlCrc32(pBuffer, dwReadedBytes, 0);

	if (dwCRC != dwCheckCRC)
	{
		return FALSE;
	}

	*pOutBuffer = pBuffer;
	*dwBytesReaded = dwReadedBytes;

	return TRUE;
}