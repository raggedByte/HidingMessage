#pragma once

#include "windows.h"

#define EMBED_MESSAGE 1
#define RETRIEVE_MESSAGE 2
#define SHOW_HELP 3

#define UNEXPECTED_PARAMS -1
#define NOT_ENOUGH_PARAMS -2

INT ProcessParams(INT argc, PWSTR argv[]);