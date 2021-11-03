#include "../headers.h"
#include "../utils/utils.h"
#include "./redirection.h"

void free_command(){

    for(int i=0; i<BUFFER_SIZE; i++){
        free(command[i]);
    }

}

void init_command(){

    for(int i=0; i<total_arguments; i++){
        command[i] = (char *)malloc(COMMAND_SIZE * sizeof(char));
    }
    command[total_arguments] = NULL;
}

int input_redirection(char *curr_command){

    int ret_value;

    // Checking for input redirection
    if(strstr(curr_command, "<") == NULL){
        return 0;
    }
    else{
        ret_value = 1;
    }

    // Getting input file name
    char *temp, *input_file, *saveptr, *saveptr2, *token;
    temp = (char *)malloc(BUFFER_SIZE * sizeof(char));

    strcpy(temp, curr_command);

    strtok_r(temp, "<", &saveptr);
    token = strtok_r(NULL, "<", &saveptr);
    input_file = strtok_r(token, " \n\r\t", &saveptr2);

    // Checking if file exists
    int is_file = 0;
    struct stat f;
    if(stat(input_file, &f) == 0 && !S_ISDIR(f.st_mode)){
        is_file = 1;
    }

    // If file does not exists:
    if(input_file == NULL || is_file == 0){
        perror("Error");
        return -1;
    }

    //Opening File
    int input_file_des;
    input_file_des = open(input_file, O_RDONLY);
    if(input_file_des < 0){
        perror("Error");
        return -1;
    }

    dup2(input_file_des, STDIN_FILENO);
    close(input_file_des);
    return ret_value;
}

int output_redirection(char *curr_command){

    int ret_value, redirection_type;

    // Checking for input redirection
    if(strstr(curr_command, ">") == NULL){
        return 0;
    }
    else{
        ret_value = 1;

        // Checking for redirection type
        if(strstr(curr_command, ">>") == NULL){
            redirection_type = 0;
        }
        else{
            redirection_type = 1;
        }  
    }

    // Getting output file name
    char *temp, *output_file, *saveptr, *saveptr2, *token;
    temp = (char *)malloc(BUFFER_SIZE * sizeof(char));

    strcpy(temp, curr_command);

    if(redirection_type == 0){
        strtok_r(temp, ">", &saveptr);
        token = strtok_r(NULL, ">", &saveptr);
    }
    else{
        strtok_r(temp, ">>", &saveptr);
        token = strtok_r(NULL, ">>", &saveptr);
    }
    output_file = strtok_r(token, " \n\r\t", &saveptr2);
    if(output_file == NULL){
        perror("Error");
        return -1;
    }

    // Checking if file exists
    int is_file = 0;
    struct stat f;
    if(stat(output_file, &f) == 0 && !S_ISDIR(f.st_mode)){
        is_file = 1;
    }

    //Opening File
    int output_file_des = 1;
    if(redirection_type == 0){
        if(is_file){
            output_file_des = open(output_file, O_WRONLY | O_TRUNC | 0644);
        }
        else{
            output_file_des = open(output_file, O_WRONLY | O_CREAT | 0644);
        }
    }
    else if(redirection_type == 1){
        if(is_file){
            output_file_des = open(output_file, O_WRONLY | O_APPEND | 0644);
        }
        else{
            output_file_des = open(output_file, O_WRONLY | O_CREAT | 0644);
        }
    }

    if(output_file_des < 0){
        perror("Error2");
        return -1;
    }

    dup2(output_file_des, STDOUT_FILENO);
    close(output_file_des);
    return ret_value;
}

void update_command_arr(char *curr_command){

    char *temp_command[BUFFER_SIZE];
    int skip, updated_num_args;

    skip = 0;
    updated_num_args = 0;

    for(int i = 0;i < total_arguments; i++){

        if(skip == 1){
            skip--;
        }
        else if(command[i][0] == '<'){

            int command_length;
            command_length = strlen(command[i]);

            if(command_length == 1){
                skip++;
            }
        }
        else if(command[i][0] == '>'){

            int command_length;
            command_length = strlen(command[i]);

            if(command_length == 1){
                skip++;
            }
            else if(command_length == 2){
                if(command[i][1] == '>'){
                    skip++;
                }
            }
        }
        else{

            temp_command[updated_num_args] = (char *)malloc(COMMAND_SIZE * sizeof(char));
            strcpy(temp_command[updated_num_args], command[i]);
            updated_num_args = updated_num_args + 1;

        }
    }

    total_arguments = updated_num_args;

    free_command();
    init_command();

    for(int i=0; i<total_arguments; i++){
        strcpy(command[i], temp_command[i]);
    }

    strcpy(commands[commands_index], command[0]);
    for(int i=0; i<total_arguments; i++){

        strcat(commands[commands_index], " ");
        strcat(commands[commands_index], command[i]);
    }

}


void redirection(char *curr_command){
    
    // Saving copies of original input/output streams
    int original_STDOUT, original_STDIN;
    original_STDIN = dup(STDIN_FILENO);
    original_STDOUT = dup(STDOUT_FILENO);

    // Checking Input Redirection
    int check_input;
    check_input = input_redirection(curr_command);
    if(check_input < 0){    
        return;                             // Exiting if any errors detected
    }

    // Checking Output Redirection
    int check_output;
    check_output = output_redirection(curr_command);
    if(check_output < 0){
        return;                             // Exiting if any errors detected
    }

    
    update_command_arr(curr_command);
    execute(command[0]);

    dup2(original_STDIN, STDIN_FILENO);
    close(original_STDIN);
    dup2(original_STDOUT, STDOUT_FILENO);
    close(original_STDOUT);

}