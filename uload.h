#pragma once


void __attribute__((noreturn)) transfer_to_user(void);
int load_user_program(const char *name);
void on_user_exit(void);
