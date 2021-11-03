#include "../headers.h"
#include "../utils/utils.h"
#include "../prompt/prompt.h"
#include "./other_commands.h"

// Creates foreground process
void makeFG(){

    // Forking
    pid_t pid;
    pid = fork();

    // Checks if error
    if(pid < 0){
        perror("fork error");
    }   
    else if(pid == 0){              // Child process

        // executing command
        int execvp_check;
        execvp_check = execvp(command[0], command);

        if(execvp_check < 0){
            perror("exec error");
        }

        exit(0);
    }
    else{                           // Parent process

        // Adding process FG_process
        strcpy(FG_process.process_name, commands[commands_index]);
        FG_process.pid = pid;

        int status;
        waitpid(pid, &status, WUNTRACED);
    }
}

// Creates background process
void makeBG(){

    // Forking
    pid_t pid;
    pid = fork();

    // Checks if error
    if(pid < 0){
        perror("fork error");
    }
    else if(pid == 0){              // Child process            

        setpgid(0, 0);

        // executing command
        int execvp_check;
        execvp_check = execvp(command[0], command);

        if(execvp_check == -1){
            perror("exec error");
        }

        exit(0);
    }
    else{
        // Adding process to linked list
        char *temp = (char *)malloc(COMMAND_SIZE * sizeof(char));
        strcpy(temp, commands[commands_index]);
        temp[strlen(commands[commands_index]) - 3] = 0;
        
        add_process(temp, pid);
        free(temp);
        total_bg_process += 1;
        printf("%u\n", pid);
    }

}

// This function is executed upon process termination (called by system automatically)
void Terminate_Process(int sig){

    int flag;
    pid_t pid;

    pid = waitpid(-1, &flag, WNOHANG | WUNTRACED);
    if(pid > 0){

        char *process_name;
        process_name = (char *)malloc(BUFFER_SIZE * sizeof(char));

        process_name = get_name(pid);

        delete_process(pid);
        total_bg_process -= 1;

        if(WIFEXITED(flag)){
            fprintf(stderr, "%s with pid = %d exited normally\n", process_name, pid);
        }
        else{
            fprintf(stderr, "%s with pid = %d exited abnormally\n", process_name, pid);
        }

        prompt();
        fflush(stdout);

    }
}

// Driver function
void other_commands(char *curr_command){

    // Checking if BG or Fg
    if(command[total_arguments - 1][0] == '&'){
        command[total_arguments - 1] = 0;
        makeBG(curr_command);
    }
    else{
        makeFG(curr_command);
    }


}