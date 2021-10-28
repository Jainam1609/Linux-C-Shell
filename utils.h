#ifndef __UTILS_H
#define __UTILS_H

#define MAX_HOSTNAME 256
#define MAX_USERNAME 256
#define MAX_PWD 256
#define MAX_FILE_NAME 256

#define PATH_SIZE 1024
#define BUFFER_SIZE 1024
#define COMMAND_SIZE 1024
#define DIR_SIZE 1024

extern char *HOME_DIR;
extern char *PREV_DIR;
extern char *prev_command;
extern char *commands[BUFFER_SIZE];
extern char *command[BUFFER_SIZE];

extern int total_commands;
extern int total_arguments;
extern int total_bg_process;
extern int commands_index;
extern int command_successful;
extern int num_history;

int tokenise_all_commands(char *str);
int tokenise_single_command(char *str);
void clear_command();
void init();
void execute(char *curr_command);

void add_process(char *process_name, int pid);
void delete_process(int pid);
char *get_name(int pid);

typedef struct Process Process;

struct Process{
    int pid;
    char process_name[BUFFER_SIZE];
    Process* next;
};

extern Process *head;
extern Process FG_process;

typedef struct Jobs Jobs;

struct Jobs{
    int job_num;
    char process_name[BUFFER_SIZE];
    char state[101];
    int pid;
};

#endif
