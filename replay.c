#include "headers.h"
#include "utils.h"
#include "bg.h"

void replay(){

    int interval, period;

    for(int i=0; i<total_arguments; i++){
        
        if(strcmp("-interval", command[i]) == 0){
            interval = atoi(command[i+1]);
        }
        else if(strcmp("-period", command[i]) == 0){
            period = atoi(command[i+1]);
        }
    }

    int num_replay = period/interval;
    int time_left = period - (num_replay * interval);

    if(num_replay == 0){
        sleep(period);
    }
    else{
        char *temp_command = (char *)malloc(COMMAND_SIZE * sizeof(char));
        strcpy(temp_command, "");

        int flag=0;
        for(int i=0; i<total_arguments; i++){
            
            if(strcmp("-command", command[i]) == 0){
                flag = 1;
                continue;
            }
            
            if(strcmp("-interval", command[i]) == 0){
                flag  = 0;
                break;
            }

            if(flag == 1){
                strcat(temp_command, command[i]);
                strcat(temp_command, " ");
            }
        }

        temp_command[strlen(temp_command)-1] = 0;
        strcpy(commands[commands_index], temp_command);
        
        char *temp = (char *)malloc(strlen(temp_command) * sizeof(char));
        for(int i=0; i<num_replay; i++){
            sleep(interval);
            strcpy(temp, temp_command);
            total_arguments = tokenise_single_command(temp);
            execute(commands[commands_index]);
        }

        free(temp);
        free(temp_command);

        sleep(time_left);
    }
}