#include "../headers.h"
#include "../utils/utils.h"
#include "./signal.h"
#include "../prompt/prompt.h"

void ctrlc(int sig){

    if(FG_process.pid > 0){

        kill(FG_process.pid, SIGINT);

        FG_process.pid = -1;
        FG_process.process_name[0] = 0;

        printf("\r");
        return;
    }

    printf("\b\b  \n");
    prompt();
    fflush(stdout);
}

void ctrlz(int sig){

    if(FG_process.pid > 0){

        kill(FG_process.pid, SIGTSTP);

        FG_process.process_name[strlen( FG_process.process_name)-1] = 0;
        add_process(FG_process.process_name, FG_process.pid);
        total_bg_process += 1;

        FG_process.pid = -1;
        FG_process.process_name[0] = 0;

        printf("\r");
        return;
    }
    printf("\b\b  \n");
    prompt();
    fflush(stdout);
}
