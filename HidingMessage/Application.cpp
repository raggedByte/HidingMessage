#include "stdio.h"

#include "ArgsHandler.h"
#include "ProcessingBMP.h"


int wmain(INT argc, PWSTR argv[])
{
	INT parsedCommand = ProcessParams(argc, argv);

	if (parsedCommand == NOT_ENOUGH_PARAMS || parsedCommand == UNEXPECTED_PARAMS)
	{
		printf("Program exectued with ");
		printf(parsedCommand == NOT_ENOUGH_PARAMS ? "not enough " : "unexpected ");
		printf("params! Try use -help for get help!\r\n");

		return -1;
	}

	if (parsedCommand == SHOW_HELP)
	{
		printf("Help:\n\
\t - help - get help!\n\
\t - embed <PathToBMP> <PathToSecretMessage> - embed secret message from file in bmp\n\
\t - retrieve <PathToBMP> <PathToFile> - retrieve secret message from BMP and copy in file\n");

		return 0;
	}
	
	PBYTE pBuffer;
	INT dwReadedBytes;
	LPCWSTR lpszPathToBMP = argv[2];
	LPCWSTR lpszPathToFile = argv[3];

	PBMP bmp = OpenBMP(lpszPathToBMP);

	if (bmp == NULL)
	{
		printf("Cannot open .BMP file!\r\n");
		return -1;
	}

	printf(".BMP file was openned!\r\n");

	switch (parsedCommand)
	{
	case EMBED_MESSAGE:
		if (!EmbedMessageInBMP(lpszPathToFile, bmp))
		{
			printf("Cannot embed (or open) secret file to .BMP file!\r\n");
		}
		else
		{
			printf("Message embed into bmp struct! Trying save . . .\r\n");
			if (!SaveBMP(lpszPathToBMP, bmp))
			{
				printf("Cannot save crypted .BMP file!\r\n");
			}
			else
			{
				printf("Done! Bmp struct was saved to .BMP file!\r\n");
			}
		}
		break;

	case RETRIEVE_MESSAGE:
		if (!RetrieveMessageFromBMP(bmp, (PVOID*)&pBuffer, &dwReadedBytes))
		{
			printf("Cannot retrieve secret file from .BMP file!\r\n");
		}
		else
		{
			printf("Message was retrieved from .BMP file! Trying save . . .\r\n");

			HANDLE hFile;

			hFile = CreateFile(lpszPathToFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile == INVALID_HANDLE_VALUE)
			{
				printf("Cannot open/create file for retrieved message!\r\n");
				return -1;
			}

			if (!WriteFile(hFile, pBuffer, dwReadedBytes, NULL, NULL))
			{
				printf("Cannot write retrieved message to file!\r\n");
				return -1;
			}

			printf("Done! Secret message retrieved into \"%ws\" file\r\n", lpszPathToFile);

			CloseHandle(hFile);
		}
		break;
	}

	CloseBMP(bmp);
	printf(".BMP file was closed!\r\n");

	return 0;
}