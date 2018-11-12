#pragma once

#ifndef NULL
#define NULL ((void *)0)
#endif
#define offsetof(type, memb) ((unsigned long) &(((type *) 0)->memb))
