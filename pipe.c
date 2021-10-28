#include "headers.h"
#include "utils.h"
#include "pipe.h"

char *pipe_command[BUFFER_SIZE];

int tokenise_pipe(char *str){
    pipe_command[0] = (char *)malloc(COMMAND_SIZE*sizeof(char));

    char *saveptr;
    char *token;

    int total_pipes = 0;
    for(token = strtok_r(str, "|", &saveptr); token != NULL; token = strtok_r(NULL, "|", &saveptr)){
        pipe_command[total_pipes] = (char *)malloc(COMMAND_SIZE*sizeof(char));
        strcpy(pipe_command[total_pipes], token);
        total_pipes += 1;
    }

    return total_pipes;
}

void piped(char *curr_command){
    
    // Initialising requires variables
    int total_pipes = 0, pipe_arr[2], prev_read_end, original_STDIN, original_STDOUT, i = 0;
    total_pipes = tokenise_pipe(curr_command);
    prev_read_end = STDIN_FILENO;
    original_STDIN = dup(STDIN_FILENO);
    original_STDOUT = dup(STDOUT_FILENO);

    // Piping the inputs and outputs of commands
    while(i<total_pipes){
        
        int pipe_check;
        pipe_check = pipe(pipe_arr);
        if(pipe_check < 0){
            perror("Piping Error");
            return;
        }

        pid_t pid = fork();

        if(pid < 0){
            perror("Forking Error");
            return;
        }
        else if(pid == 0){          // Child

            // Reading from output stream of previous command
            dup2(prev_read_end, STDIN_FILENO);
            close(pipe_arr[0]);     // Closing theread end of pipe

            // Writing to current pipe stream
            if(total_pipes != i + 1){
                dup2(pipe_arr[1], STDOUT_FILENO);
            }
            {
                char *temp;
                temp = (char *)malloc(COMMAND_SIZE * sizeof(char));
                strcpy(commands[commands_index], pipe_command[i]);
                strcpy(temp, pipe_command[i]);
                clear_command();
                tokenise_single_command(temp);
                execute(commands[commands_index]);
                free(temp);
            }
            exit(EXIT_SUCCESS);

        }
        else{                               // Parent

            wait(NULL);                     // Waiting for Child Process
            close(pipe_arr[1]);             // Closing the write end of pipe 
            prev_read_end = pipe_arr[0];    // Storing ead end of command  

        }
        i++;
    }
    
    dup2(original_STDIN, STDIN_FILENO);
    dup2(original_STDOUT, STDOUT_FILENO);
}