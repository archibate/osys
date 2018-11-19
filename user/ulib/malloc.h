#pragma once

#include <sizet.h>

void *malloc(size_t size);
void free(void *p);

#define malloc_for(type) malloc(sizeof(type))
