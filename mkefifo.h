#pragma once

#include <fs.h>
#include <efifo.h>

INODE *make_efifo_dev(const char *name, EFIFO *efifo, unsigned int iattr);
