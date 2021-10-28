#ifndef __HISTORY_H
#define __HISTORY_H

#include "utils.h"
#define MAX_HISTORY 20

extern char hist_commands[MAX_HISTORY][BUFFER_SIZE];

void history();
void update_history(char *curr_command);
void save_history();
void load_history();

#endif
