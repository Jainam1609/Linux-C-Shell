#include "headers.h"
#include "utils.h"
#include "history.h"

// This function loads history upon shell invocation
char hist_commands[MAX_HISTORY][BUFFER_SIZE];

void load_history(){

    // Getting path of "history.txt"
    char *path = (char *)malloc(DIR_SIZE * sizeof(char));
    strcpy(path, HOME_DIR);
    strcat(path, "/history.txt");

    FILE* f = fopen(path, "r+");
    if(f == NULL){
        perror("history error");
        return;
    }

    // Reading contents of history.txt
    size_t buff_size = BUFFER_SIZE;
    char* buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer");
    }
    getline(&buffer, &buff_size, f);

    // Getting total number of saved commands
    num_history = atoi(buffer);

    for(int i=0; i<num_history; i++){
        getline(&buffer, &buff_size, f);
        strcpy(hist_commands[i], buffer);
    }

    free(path);
    free(buffer);
    fclose(f);

}

// This function saves commands once shell is exited
void save_history(){

    // Getting path of "history.txt"
    char *path = (char *)malloc(DIR_SIZE * sizeof(char));
    strcpy(path, HOME_DIR);
    strcat(path, "/history.txt");

    // Opening "history.txt" in write+ mode
    FILE* f = fopen(path, "w+");
    if(f == NULL){
        perror("history error");
        return;
    }

    // Storing total number of commands
    fprintf(f, "%d\n", num_history);

    // Storing commands
    for(int i=0; i<num_history; i++){
        fprintf(f, "%s", hist_commands[i]);
    }

    fclose(f);
    free(path);
}

// This function updates command history as they are read from shell
void update_history(char *curr_command){

    // Commands are stored in an array of strings
    // If 20 commands are already saved, we erase the oldest(hist_commands[0]) and append the latest command
    if(num_history == MAX_HISTORY){
        for(int i=0; i<MAX_HISTORY - 1; i++){
            strcpy(hist_commands[i], hist_commands[i+1]);
        }
        strcpy(hist_commands[MAX_HISTORY - 1], curr_command);
    }
    else{
        strcpy(hist_commands[num_history], curr_command);
        num_history += 1;
    }

}

// Driver function
void history(){

    // Only history
    if(total_arguments == 1){

        // Printing all commands if total saved commands <= 10
        if(num_history <= 10){
            for(int i=0; i<num_history; i++){
                printf("%s", hist_commands[i]);
            }
        }
        // Printing latest 10 commands
        else{
            for(int i=num_history - 10; i<num_history; i++){
            printf("%s", hist_commands[i]);
        }
        }
    }
    // history <num>
    else{
        int num = atoi(command[1]);                             // Getting number of commands to print

        // Printing saved commands
        if(num_history > num){
            for(int i=num_history - num; i<num_history; i++){
                printf("%s", hist_commands[i]);
            }
        }
        else{
            for(int i=0; i<num_history; i++){
                printf("%s", hist_commands[i]);
            }
        }
    }

}
