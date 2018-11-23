#pragma once

#include <fs.h>
#include <efifo.h>

INODE *setup_efifo_dev(INODE *inode, EFIFO *efifo);
