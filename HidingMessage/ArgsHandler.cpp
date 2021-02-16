#include "ArgsHandler.h"
#include "stdio.h"

INT ProcessParams(INT argc, PWSTR argv[])
{
	if (argc < 2)
	{
		return NOT_ENOUGH_PARAMS;
	}

	PWSTR command = argv[1];

	if (wcscmp(L"-help", command) == 0)
	{
		return SHOW_HELP;
	}
	else if (argc == 4)
	{
		if (wcscmp(L"-embed", command) == 0)
		{
			return EMBED_MESSAGE;
		}
		else if (wcscmp(L"-retrieve", command) == 0)
		{
			return RETRIEVE_MESSAGE;
		}
	}

	return argc < 4 ? NOT_ENOUGH_PARAMS : UNEXPECTED_PARAMS;
}