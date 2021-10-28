#include "headers.h"
#include "utils.h"
#include "pinfo.h"

// This function
int get_num(char *str){

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

// Prints information regarding process
void pinfo(){

    int pid;

    // getting pid
    if(total_arguments == 2){
        pid = get_num(command[1]);
    }
    else if(total_arguments == 1){
        pid = getpid();
    }
    else{
        printf("Invalid Number fo Arguments\n");
        return;
    }

    char file[PATH_SIZE];
    sprintf(file, "/proc/%d/stat", pid);

    // opening "/proc/<pid>/stat"
    FILE *fp;
    fp = fopen(file, "r");

    if(fp){
        
        // Reading through the file
        size_t path_size = PATH_SIZE;
        char *input_str = (char *)malloc(PATH_SIZE * sizeof(char));
        getline(&input_str, &path_size, fp);

        char *stat_str[100];
        stat_str[0] = (char *)malloc(COMMAND_SIZE*sizeof(char));

        char *saveptr;
        char *token;

        // tokenising read line based on " \t\r"
        int counter = 0, len;
        for(token = strtok_r(input_str, " \t\r", &saveptr); token != NULL; token = strtok_r(NULL, " \t\r", &saveptr)){
            stat_str[counter] = (char *)malloc(COMMAND_SIZE*sizeof(char));
            strcpy(stat_str[counter], token);
            counter += 1;
        }

        char process_type[2];
        process_type[1] = 0;

        // Checking if foreground or background process
        int check_fg;
        if(strcmp(stat_str[0], stat_str[7]) == 0){
            check_fg = 1;
            process_type[0] = '+';
        }else{
            check_fg = 0;
            process_type[0] = '-';
        }

        char file_2[PATH_SIZE], exec_path[PATH_SIZE];
        sprintf(file_2, "/proc/%d/exe", pid);

        // Reading the linked file "/proc/<pid>/exe"
        len = readlink(file_2, exec_path, sizeof(exec_path));

        if(len == -1){
            strcpy(exec_path, "NULL");
            exec_path[4] = '\0';
        }
        else{
            exec_path[len] = '\0';
        }

        // getting path of executable
        char final_path[PATH_SIZE];
        if(strcmp(exec_path, HOME_DIR) == 0){
            strcpy(final_path, "~");
        }
        else if(strstr(exec_path, HOME_DIR)){
            sprintf(final_path, "~%s", exec_path + strlen(HOME_DIR));
        }
        else{
            strcpy(final_path, exec_path);
        }

        // printing information regarding process
        printf("pid -- %s\n", stat_str[0]);
        printf("Process Status -- %s%s\n", stat_str[2], process_type);
        printf("memory -- %s\n", stat_str[22]);
        printf("Executable Path -- %s\n", final_path);

        free(input_str);
    }
    else{
        perror("file error");
        return;
    }
}