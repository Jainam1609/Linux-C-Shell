#include "headers.h"
#include "utils.h"
#include "jobs.h"

// This array stores input flags
int flags_jobs[2];
int num_input_flags_jobs;

void setflags_jobs(){

    flags_jobs[0] = 0;   // -r falg
    flags_jobs[1] = 0;   // -s flag
    num_input_flags_jobs = 0;

    for(int i = 1; i < total_arguments; i++){

        if(command[i][0] == '-'){

            int arg_length = strlen(command[i]);
            for(int j = 0; j < arg_length; j++){

                if(command[i][j] == 'r'){
                    flags_jobs[0] = 1;
                }
                else if(command[i][j] == 's'){
                    flags_jobs[1] = 1;
                }
            }

            num_input_flags_jobs += 1;
        }
    }
}

void jobs(char *curr_command){

    setflags_jobs();

    Jobs jobs_arr[100];

    char *path;
    path = (char *)malloc(PATH_SIZE * sizeof(char));
    Process* curr = head -> next;

    for(int i = total_bg_process; curr != NULL;curr = curr -> next, i--){

        sprintf(path, "/proc/%d/stat", curr -> pid);

        // opening "/proc/<pid>/stat"
        FILE *fp;
        fp = fopen(path, "r");

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
            int counter = 0;
            for(token = strtok_r(input_str, " \t\r", &saveptr); token != NULL; token = strtok_r(NULL, " \t\r", &saveptr)){
                stat_str[counter] = (char *)malloc(COMMAND_SIZE*sizeof(char));
                strcpy(stat_str[counter], token);
                counter += 1;
            }

            char *state;
            state = (char *)malloc(BUFFER_SIZE * sizeof(char));

            if(strcmp(stat_str[2], "I") == 0){
                strcpy(state, "Interrupted");
            }
            else if(strcmp(stat_str[2], "T") == 0){
                strcpy(state, "Stopped");
            }
            else{
                strcpy(state, "Running");
            }

            jobs_arr[i].job_num = i;
            jobs_arr[i].pid = curr -> pid;
            strcpy(jobs_arr[i].process_name, curr ->process_name);
            strcpy(jobs_arr[i].state, state);

        }
        else{
            perror("Jobs Error");
        }
    }

    // Sorting the Jobs
    for(int i = 1; i <= total_bg_process;i++)
    {
        for(int j = i+1; j <= total_bg_process;j++)
        {
            if (strcmp(jobs_arr[i].process_name, jobs_arr[j].process_name) > 0)
            {
                int temp_int = jobs_arr[i].job_num;
                jobs_arr[i].job_num = jobs_arr[j].job_num;
                jobs_arr[j].job_num = temp_int;

                temp_int = jobs_arr[i].pid;
                jobs_arr[i].pid = jobs_arr[j].pid;
                jobs_arr[j].pid = temp_int;

                char temp_char[BUFFER_SIZE];
                strcpy(temp_char, jobs_arr[i].state);
                strcpy(jobs_arr[i].state, jobs_arr[j].state);
                strcpy(jobs_arr[j].state, temp_char);

                strcpy(temp_char, jobs_arr[i].process_name);
                strcpy(jobs_arr[i].process_name, jobs_arr[j].process_name);
                strcpy(jobs_arr[j].process_name, temp_char);
            }
        }
    }

    // Printign the require Output
    for(int i = 1; i <= total_bg_process; i++){

        if((flags_jobs[0]==0 && flags_jobs[1] == 0) || flags_jobs[0]==1 && flags_jobs[1] == 1){
            printf("[%d] %s %s [%d]\n",jobs_arr[i].job_num, jobs_arr[i].state, jobs_arr[i].process_name, jobs_arr[i].pid);
        }
        else if(flags_jobs[0]==1 && flags_jobs[1] == 0){
            if(strcmp("Running", jobs_arr[i].state) == 0){
                printf("[%d] %s %s [%d]\n",jobs_arr[i].job_num, jobs_arr[i].state, jobs_arr[i].process_name, jobs_arr[i].pid);
            }
        }
        else{
            if(strcmp("Stopped", jobs_arr[i].state) == 0){
                printf("[%d] %s %s [%d]\n",jobs_arr[i].job_num, jobs_arr[i].state, jobs_arr[i].process_name, jobs_arr[i].pid);
            }
        }
    }
}
