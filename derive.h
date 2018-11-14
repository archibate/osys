#pragma once


#include <stddef.h>

#include <stddef.h> // offsetof
#define derive_of(type, base, derived) \
	((type*) ((char*)(derived) - (long)offsetof(type, base)))
