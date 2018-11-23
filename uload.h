#pragma once

struct FILE;

int exec_user_program_fc(struct FILE *f);
void user_space_destroy(void);
