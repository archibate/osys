#pragma once


#define STRUCT(name) EXTSTRUCT(name); DEFSTRUCT(name)
#define EXTSTRUCT(name) typedef struct name name
#define DEFSTRUCT(name) struct name
