#include "headers.h"
#include "utils.h"
#include "sig.h"

// This function converts a number of type string to int
int get_num_3(char *str){

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

void sig(){

    if(total_arguments != 3){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return;
    }

    int job_number;
    job_number = get_num_3(command[1]);
    if(job_number > total_bg_process || job_number <= 0){
        fprintf(stderr, "Error: Invalid Job Number\n");
        return;
    }

    int signal = get_num_3(command[2]);
    if(signal <= 0){
        fprintf(stderr, "Error: Invalid Signal Number\n");
        return;
    }

    Process* curr = head -> next;
    for(int i = total_bg_process; curr != NULL;curr = curr -> next, i--){
        if(i == job_number){
            printf("%s\n", curr->process_name);
            kill(curr ->pid, signal);
            return;
        }
    }
}