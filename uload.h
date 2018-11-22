#pragma once

struct FILE;

void __attribute__((noreturn)) transfer_to_user(int spoff);
int load_user_program(const char *name);
int load_user_program_fc(struct FILE *f);
void user_proc_destroy(void);
