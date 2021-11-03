#include "cd.h"
#include "../headers.h"
#include "../utils/utils.h"

#define PATH_SIZE 1024

void cd(){
    
    int check;
    command_successful = 1;

    char *TEMP_DIR = (char *)malloc(DIR_SIZE * sizeof(char));
    getcwd(TEMP_DIR, DIR_SIZE);

    // Invalid number of arguments
    if(total_arguments > 2){
        printf("cd error: too many arguments\n");
    }
    else if(total_arguments == 1){                      // Only cd 
        check = chdir(HOME_DIR);
        if(check != 0){
            perror("cd error");
            command_successful = 0;
        }
    }
    else if(command[1][0] == '~'){                      
        
        if(strlen(command[1]) == 1){                    // cd ~
            check = chdir(HOME_DIR);
            if(check != 0){
                perror("cd error");
                command_successful = 0;
            }
        }
        else{                                           // cd ~/<dir_name>
            char *final_dir = (char *)malloc(MAX_PWD * sizeof(char));
            strcpy(final_dir, HOME_DIR);
            strcat(final_dir, command[1] + 1);

            check = chdir(final_dir);
            if(check != 0){
                perror("cd error");
                command_successful = 0;
            }
        }
    }
    else if(command[1][0] == '-'){

        if(strlen(command[1]) == 1){                    // cd -

            printf("%s\n", PREV_DIR);
            check = chdir(PREV_DIR);
            if(check != 0){
                perror("cd error");
                command_successful = 0;
            }
        }
        else{
            check = chdir(command[1]);
            if(check != 0){
                perror("cd error");
                command_successful = 0;
            }
        }
    }
    else{
        check = chdir(command[1]);
        if(check != 0){
            perror("cd error");
            command_successful = 0;
        }
    }

    // Updating PREV_DIR
    if(command_successful){
        strcpy(PREV_DIR, TEMP_DIR);
    }
    free(TEMP_DIR);
    
}   