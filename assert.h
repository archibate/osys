#pragma once


#define assert(x) \
	if (!(x)) { asm volatile ("cli; hlt" : "a" (0xdeadbeaf)); } else {}
