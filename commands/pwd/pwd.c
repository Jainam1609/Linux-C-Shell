#include "../headers.h"
#include "../utils/utils.h"
#include "./pwd.h"

void pwd(){

    char *pwd = (char *)malloc(DIR_SIZE * sizeof(char));
    char *check;

    // Getting current working directory
    check = getcwd(pwd, DIR_SIZE);
    if(check != NULL){
        printf("%s\n", pwd);
    }
    else{
        perror("pwd error");
        command_successful = 0;
    }

    command_successful = 1;

}