#pragma once

#include "windows.h"

typedef struct tagBMP
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	PRGBTRIPLE bitmap;
	PBYTE pWriter;
	PBYTE pReader;
}BMP, *PBMP;

PBMP OpenBMP(LPCWSTR lpszFilePath);
BOOL CloseBMP(PBMP pBMP);
BOOL SaveBMP(LPCWSTR lpszFilePath, PBMP pBmp);