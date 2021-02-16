#include "bmp.h"
#include "stdio.h"

PBMP OpenBMP(LPCWSTR lpszFilePath)
{
	HANDLE hFileBMP;
	DWORD dwReadedBytes;
	LONG biWidthBMP, biHeightBMP;
	PBMP pBmp;

	pBmp = (PBMP)malloc(sizeof(BMP));

	if (pBmp == NULL)
	{
		return NULL;
	}

	hFileBMP = CreateFile(lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileBMP == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	if (!ReadFile(hFileBMP, &(pBmp->fileHeader), sizeof(BITMAPFILEHEADER), &dwReadedBytes, NULL))
	{
		return NULL;
	}

	if (!ReadFile(hFileBMP, &(pBmp->infoHeader), sizeof(BITMAPINFOHEADER), &dwReadedBytes, NULL))
	{
		return NULL;
	}

	biWidthBMP = pBmp->infoHeader.biWidth;
	biHeightBMP = pBmp->infoHeader.biHeight;

	SetFilePointer(hFileBMP, pBmp->fileHeader.bfOffBits, NULL, FILE_BEGIN);

	pBmp->bitmap = (PRGBTRIPLE)malloc(sizeof(RGBTRIPLE) * biWidthBMP * biHeightBMP);

	if (pBmp->bitmap == NULL)
	{
		return NULL;
	}

	pBmp->pWriter = (PBYTE)pBmp->bitmap;
	pBmp->pReader = (PBYTE)pBmp->bitmap;

	for (INT i = 0; i < biHeightBMP; i++)
	{
		for (INT j = 0; j < biWidthBMP; j++)
		{
			if (!ReadFile(hFileBMP, (pBmp->bitmap + (DWORD)(i * biWidthBMP + j)), sizeof(RGBTRIPLE), &dwReadedBytes, NULL))
			{
				return NULL;
			}
		}
		SetFilePointer(hFileBMP, biWidthBMP % 4, NULL, FILE_CURRENT);
	}

	CloseHandle(hFileBMP);
	return pBmp;
}

BOOL SaveBMP(LPCWSTR lpszFilePath, PBMP pBmp)
{
	HANDLE hFile;
	DWORD dwWrittenBytes;
	LONG biWidthBMP, biHeightBMP;
	BYTE carryByte[4];

	memset(carryByte, 0, sizeof(BYTE) * 4);

	hFile = CreateFile(lpszFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error while oppening file!\n");
		return FALSE;
	}

	biWidthBMP = pBmp->infoHeader.biWidth;
	biHeightBMP = pBmp->infoHeader.biHeight;

	if (!WriteFile(hFile, &(pBmp->fileHeader), sizeof(BITMAPFILEHEADER), &dwWrittenBytes, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}

	if (!WriteFile(hFile, &(pBmp->infoHeader), sizeof(BITMAPINFOHEADER), &dwWrittenBytes, NULL))
	{
		printf("Cannot write in file!\n");
		return FALSE;
	}

	for (INT i = 0; i < biHeightBMP; i++)
	{
		for (INT j = 0; j < biWidthBMP; j++)
		{
			if (!WriteFile(hFile, (pBmp->bitmap + (i * biWidthBMP + j)), sizeof(RGBTRIPLE), &dwWrittenBytes, NULL))
			{
				printf("Cannot write in file!\n");
				return FALSE;
			}
		}

		if (!WriteFile(hFile, &carryByte, biWidthBMP % 4, &dwWrittenBytes, NULL))
		{
			printf("Cannot write in file!\n");
			return FALSE;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}

BOOL CloseBMP(PBMP pBmp)
{
	free(pBmp->bitmap);
	free(pBmp);

	return TRUE;
}