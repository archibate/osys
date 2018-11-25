#pragma once

#include <fsdefs.h>
#include <fstyps.h>

struct U_DIRENT
{
	ino_t e_ino;
	char e_name[MAX_FNAME + 1];
};
