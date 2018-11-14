#pragma once


void *kmalloc(unsigned long len);
void kfree(void *p);

#define kmalloc_for(type) ((type*)kmalloc(sizeof(type)))
