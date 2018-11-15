#pragma once


#ifndef _SIZE_T_DEFINED
typedef unsigned long size_t;
#endif
#ifndef _SSIZE_T_DEFINED
typedef signed long ssize_t;
#endif
typedef unsigned int off_t;

typedef unsigned int clus_t; // use fat of 32bit in ramfs, (although 12bit physically)
