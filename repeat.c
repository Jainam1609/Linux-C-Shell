#include "headers.h"
#include "utils.h"
#include "repeat.h"

// This function converts a number of type string to int
int get_num_2(char *str){

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

void repeat(){

    if(total_arguments == 1){
        printf("error: Invalid number of arguments\n");
        return;
    }

    // Converting number of type string to int
    int num = get_num_2(command[1]);

    // Making array required for execvp
    for(int i = 2; i<total_arguments; i++){
        strcpy(command[i - 2], command[i]);
    }
  
    strcpy(command[total_arguments - 1], "");
    strcpy(command[total_arguments - 2], "");

    total_arguments = total_arguments - 2;

    // executing commands
    for(int i=0; i<num;i++){
        execute(command[0]);
    }
}