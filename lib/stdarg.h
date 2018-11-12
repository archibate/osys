#pragma once

#ifdef __GNUC__

#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end

#else

#define __$imaxsizeof(type) (sizeof(head) > sizeof(int) ? sizeof(head) : sizeof(int))
typedef char *va_list;
#define va_start(ap, head) ((ap) = (va_list) &(head) + __$imaxsizeof(head))
#define va_arg(ap, type) (*(type *)(((ap) += __$imaxsizeof(type)) - __$imaxsizeof(type)))
#define va_end(ap) ((ap) = (va_list) 0)

#endif
