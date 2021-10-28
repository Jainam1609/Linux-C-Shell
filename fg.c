#include "headers.h"
#include "utils.h"
#include "bg.h"

// This function converts a number of type string to int
int get_num_5(char *str){

    int num = 0, single_num;
    for(int i=0; i<strlen(str); i++){
        num *= 10;
        single_num = str[i] - '0';
        if(!(single_num >= 0  && single_num <= 9)){
            perror("wrong input");
            return -1;
        }
        num += single_num;
    }

    return num;
}

void fg(){

    if(total_arguments != 2){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return;
    }

    int job_number;
    job_number = get_num_5(command[1]);
    if(job_number > total_bg_process || job_number <= 0){
        fprintf(stderr, "Error: Invalid Job Number\n");
        return;
    }

    Process* curr = head -> next;
    for(int i = total_bg_process; curr != NULL;curr = curr -> next, i--){
        if(i == job_number){

            int pid = curr -> pid;
            char *p_name;
            p_name = (char *)malloc(BUFFER_SIZE * sizeof(char));
            strcpy(p_name, curr ->process_name);

            strcpy(FG_process.process_name, p_name);
            FG_process.pid = pid;

            delete_process(curr -> pid);
            total_bg_process -= 1;

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            tcsetpgrp(STDIN_FILENO, pid);

            kill(pid, SIGCONT);

            int status;
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if(WIFSTOPPED(status)){
                add_process(p_name, pid);
                total_bg_process += 1;

                FG_process.pid = -1;
                FG_process.process_name[0] = 0;
            }

            return;
        }
    }

    printf("Error: Invalid Job Number\n");
}