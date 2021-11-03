#include "../headers.h"
#include "../cd/cd.h"
#include "./utils.h"
#include "../echo/echo.h"
#include "../pwd/pwd.h"
#include "../ls/ls.h"
#include "../history/history.h"
#include "../other_commands/other_commands.h"
#include "../pinfo/pinfo.h"
#include "../pinfo/pinfo.h"
#include "../redirection/redirection.h"
#include "../pipe/pipe.h"
#include "../jobs/jobs.h"
#include "../sig/sig.h"
#include "../fg/fg.h"
#include "../bg/bg.h"
#include "../signal/signal.h"
#include "../replay/replay.h" 
#include "../repeat/repeat.h"

char *HOME_DIR;
char *PREV_DIR;
char *prev_command;
char *commands[BUFFER_SIZE];
char *command[BUFFER_SIZE];

int total_commands;
int total_arguments;
int total_bg_process;
int commands_index;
int command_successful;
int num_history;

Process *head;
Process FG_process;
// This function tokenisez input buffer based on ';'
int tokenise_all_commands(char *str){

    commands[0] = (char *)malloc(COMMAND_SIZE*sizeof(char));

    char *saveptr;
    char *token;

    int total_commands = 0;
    for(token = strtok_r(str, ";", &saveptr); token != NULL; token = strtok_r(NULL, ";", &saveptr)){
        commands[total_commands] = (char *)malloc(COMMAND_SIZE*sizeof(char));
        strcpy(commands[total_commands], token);
        total_commands += 1;
    }

    return total_commands;
}

// This function tokenisez input command based on " \n\t\r"
int tokenise_single_command(char *str){

    command[0] = (char *)malloc(COMMAND_SIZE*sizeof(char));

    char *saveptr;
    char *token;

    total_arguments = 0;
    for(token = strtok_r(str, " \n\t\r", &saveptr); token != NULL; token = strtok_r(NULL, " \n\t\r", &saveptr)){
        command[total_arguments] = (char *)malloc(COMMAND_SIZE*sizeof(char));
        strcpy(command[total_arguments], token);
        total_arguments += 1;
    }

    return total_arguments;
}

// Adding process to the linked list
void add_process(char *process_name, int pid){

    Process* temp = (Process *)malloc(sizeof(Process));
    strcpy(temp -> process_name, process_name);
    temp -> pid = pid;

    temp -> next = head -> next;
    head -> next = temp;

}

// Deleting process to the linked list
void delete_process(int pid){

    // Store head node
    Process *temp = head, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->pid == pid) {
        head = temp->next; // Changed head
        free(temp); // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->pid != pid) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory

}

// Getting name of process based on pid
char *get_name(int pid){

    char *name = (char *)malloc(BUFFER_SIZE * sizeof(char));

    for(Process* curr = head; curr = curr -> next; curr -> next != NULL){
        if(curr -> pid == pid){
            strcpy(name, curr -> process_name);
            break;
        }
    }
    return name;
}

// Initialising all extern variable
void init(){

    HOME_DIR = (char *)malloc(DIR_SIZE*sizeof(char));
    getcwd(HOME_DIR, DIR_SIZE);
    PREV_DIR = (char *)malloc(DIR_SIZE*sizeof(char));
    getcwd(PREV_DIR, DIR_SIZE);
    prev_command = (char *)malloc(BUFFER_SIZE * sizeof(char));

    head = (Process *)malloc(sizeof(Process));
    strcpy(head -> process_name, "");
    head -> pid = 0;
    head -> next = NULL;

    total_bg_process = 0;

    load_history();

    strcpy(prev_command, hist_commands[num_history-1]);
}

int is_redirection(char *curr_command){

    int ret_val = -1;
    if(strstr(curr_command, ">") != NULL || strstr(curr_command, "<") != NULL){
        ret_val = 1;
    }

    return ret_val;

}

int is_pipe(char *curr_command){

    int ret_val = -1;
    if(strstr(curr_command, "|") != NULL){
        ret_val = 1;
    }

    return ret_val;

}

void clear_command(){

    for(int i = 0; i < total_arguments; i++){
        command[i] = 0;
    }
    
}

// Executing all commands
void execute(char *curr_command){

    // Checking if command is piped
    if(is_pipe(curr_command) == 1){
        piped(curr_command);
        clear_command();
        return;
    }

    // Checking if command requires Redirection
    if(is_redirection(curr_command) == 1){
        redirection(curr_command);
        clear_command();
        return;
    }

    if(strcmp(command[0], "cd") == 0){
        cd();
    }
    else if(strcmp(command[0], "echo") == 0){
        echo();
    }
    else if(strcmp(command[0], "pwd") == 0){
        pwd();
    }
    else if(strcmp(command[0], "ls") == 0){
        ls();
    }
    else if(strcmp(command[0], "history") == 0){
        history();
    }
    else if(strcmp(command[0], "pinfo") == 0){
        pinfo();
    }
    else if(strcmp(command[0], "exit") == 0){
        save_history();
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(command[0], "repeat") == 0){
        repeat();
    }
    else if(strcmp(command[0], "jobs") == 0){
        jobs(curr_command);
    }
    else if(strcmp(command[0], "sig") == 0){
        sig();
    }
    else if(strcmp(command[0], "fg") == 0){
        fg();
    }
    else if(strcmp(command[0], "bg") == 0){
        bg();
    }
    else if(strcmp(command[0], "replay") == 0){
        replay();
    }
    else{
        other_commands(curr_command);
    }

    clear_command();

}
