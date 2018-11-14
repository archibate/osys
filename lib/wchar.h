#pragma once


#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif

#define wch2cch(wc) ((wc) & 0xff)
