#include "prompt.h"
#include "headers.h"
#include "utils.h"
#include "history.h"
#include "other_commands.h"
#include "signal.h"


int main(){

    // Initialising all the extern variable
    init();

    signal(SIGCHLD, Terminate_Process);     // Catches any terminated background process and calls Terminate_Process()
    signal(SIGINT, ctrlc);
    signal(SIGTSTP, ctrlz);

    while (1){

        // Printing the prompt
        prompt();

        // Getting input from user
        size_t buff_size = BUFFER_SIZE;
        char* buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
        if(buffer == NULL){
            perror("Unable to allocate buffer");
        }

        if(getline(&buffer, &buff_size, stdin) == -1){
            save_history();
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        if (!strcmp(buffer, "\n"))
            continue;

        // Updating command history
        if(strcmp(prev_command, buffer) != 0){
            update_history(buffer);
        }

        // Tokenising input command based in ';'
        total_commands = tokenise_all_commands(buffer);

        // Traversing through all the commands
        commands_index = 0;
        char *temp = (char *)malloc(BUFFER_SIZE * sizeof(char));
        for(; commands_index < total_commands; commands_index++){
            strcpy(temp, commands[commands_index]);
            total_arguments = tokenise_single_command(temp);     // tokenising each command based on " \n\t\r"
            execute(commands[commands_index]);                   // executing the command
        }
        free(temp);

        // updating the variable storing previous command
        strcpy(prev_command, buffer);

        // Making all the commands as NULL
        for(int i=0; i<total_commands;i++){
            commands[i] = 0;                                        // executing the command
        }
        free(buffer);
    }

    return 0;
}
