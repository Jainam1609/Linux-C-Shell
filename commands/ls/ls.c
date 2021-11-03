#include "../headers.h"
#include "../utils/utils.h"
#include "./ls.h"


int flags[2] = {0};         // stores info about flags. flags[0] is -a and flags[1] is -l
int num_input_flags;        // keeps track of number of flag arguments in input
int num_input_dirs;         // keeps track of number of directories in input
int idx;                    // keeps track of index number while traversing command
int non_flag_ls;            // Number of arguments except flags and "ls"
int first_ls_l_print;       // Storing if any ls -l file/directory is printed

// This function gets file name from absolute path
char *get_file_name(char *dir){

    char *file_name = (char *)malloc(MAX_FILE_NAME * sizeof(char));

    // Finding last position of '/'
    int pos = -1;
    for(int i=0 ;i<strlen(dir); i++){
        if(dir[i] == '/'){
            pos = i;
        }
    }
    
    if(pos == -1){
        strcpy(file_name, dir);
    }
    else{
        
        char temp_name[strlen(dir)];
        for(int i = 0; i<strlen(dir); i++){
           temp_name[i] = dir[i+pos+1];
        }
        strcpy(file_name, temp_name);
    }
    
    return file_name;

}

// This function gets absolute path 
char* get_abs_path(char *given_path){

    char *temp_path = (char *)malloc(DIR_SIZE * sizeof(char));

    // Checking if relative path
    if(given_path[0] == '~'){
        strcpy(temp_path, HOME_DIR);
        strcat(temp_path, given_path + 1);
    }
    else{
        struct dirent *d;
        DIR *reader = opendir(given_path);

        // Checking if directory
        if(reader == NULL){
            strcpy(temp_path,HOME_DIR);
            strcat(temp_path, "/");
            strcat(temp_path, given_path);
        }
        else{
            strcpy(temp_path, given_path);
        }
    }

    return temp_path;
}

// This function checks the input flags, i.e., -a and -l  flags
void setflags(){

    flags[0] = 0;
    flags[1] = 0;
    num_input_flags = 0;
    num_input_dirs = 0;
    non_flag_ls = 0;

    for(int i=1; i < total_arguments; i++){

        if(command[i][0] == '-'){

            int arg_length = strlen(command[i]);
            for(int j = 0; j < arg_length; j++){

                if(command[i][j] == 'a'){
                    flags[0] = 1;
                }
                else if(command[i][j] == 'l'){
                    flags[1] = 1;
                }
            }

            num_input_flags += 1;
        }
        else{

            char *INPUT_DIR = (char *)malloc(DIR_SIZE * sizeof(char));
            INPUT_DIR = get_abs_path(command[i]);

            struct dirent *d;
            DIR *reader = opendir(INPUT_DIR);

            // Checking if directory
            if(reader != NULL){
                num_input_dirs+=1;
            }

            non_flag_ls += 1;

            closedir(reader);
        }
    }
}

// This function prints only ls
void normal_ls(char *dir){
    
    struct dirent *d;
    DIR *reader = opendir(dir);

    // Checking if directory
    if(reader == NULL){

        // Checking if file
        if(access(dir, F_OK) == 0){
            printf("%s ", get_file_name(dir));
        }
        else{
            perror("ls error");
        }

    }
    else{

        // Traversing through files of directory and printing
        while((d = readdir(reader)) != NULL){
            if(d->d_name[0] != '.'){
                printf("%s ", d -> d_name);
            }
        }
        printf("\n");

        if(num_input_dirs >= 2 && idx != total_arguments - 1){
            printf("\n");
        }
    }

    

}

// This function prints only ls -l
void ls_l(char *dir){

    struct dirent *d;
    DIR *reader = opendir(dir);
    
    struct dirent *d_temp;
    DIR *reader_temp = opendir(dir);

    // Checking if directory
    if(reader == NULL){

        // Checking if file
        // This is for ls -l <file_name>
        if(access(dir, F_OK) == 0){

            char perms[12] = "", time[30];
            
            struct stat s;
            lstat(dir, &s);

            struct passwd *pw;
            pw = getpwuid(s.st_uid);

            struct group *gr;
            gr = getgrgid(s.st_gid);
            
            // Getting file permissions
            S_ISDIR(s.st_mode) ? strcat(perms, "d") : strcat(perms, "-");
            (s.st_mode & S_IRUSR) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWUSR) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXUSR) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IRGRP) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWGRP) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXGRP) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IROTH) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWOTH) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXOTH) ? strcat(perms, "x") : strcat(perms, "-");
            
            // Getting last modified time and date
            // Date/Time format is "Month Date Hour:Minute"
            strftime(time, 30, "%b %d %H:%M", localtime(&(s.st_mtime)));
               
            if(first_ls_l_print == 0){
                printf("%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, get_file_name(dir)); 
                first_ls_l_print = 1;
            }
            else{
                printf("\n%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, get_file_name(dir)); 
            }
        }
        else{
            perror("ls error");
        }
    }
    else{ // This is for ls -l <dir_name>

        int size = 0;

        // Traversing through all files of directory to find total block size of all files
        while((d_temp = readdir(reader_temp)) != NULL){

            // Ignoring .files
            if(d_temp->d_name[0] != '.'){               
                char *file_path = (char *)malloc(DIR_SIZE * sizeof(char));
                strcpy(file_path, dir);
                strcat(file_path, "/");
                strcat(file_path, d_temp -> d_name);

                struct stat s;
                lstat(file_path, &s);

                size += s.st_blocks;
            }
        }

        printf("total %d\n", size);

        // Traversing through all files of directory to print file perms and details
        while((d = readdir(reader)) != NULL){

            // Ignoring .files
            if(d->d_name[0] != '.'){

                // Getting file path
                char *file_path = (char *)malloc(DIR_SIZE * sizeof(char));
                strcpy(file_path, dir);
                strcat(file_path, "/");
                strcat(file_path, d -> d_name);

                char perms[12] = "", time[30];

                struct stat s;
                lstat(file_path, &s);

                struct passwd *pw;
                pw = getpwuid(s.st_uid);

                struct group *gr;
                gr = getgrgid(s.st_gid);

                // Getting file/dir permissions
                S_ISDIR(s.st_mode) ? strcat(perms, "d") : strcat(perms, "-");
                (s.st_mode & S_IRUSR) ? strcat(perms, "r") : strcat(perms, "-");
                (s.st_mode & S_IWUSR) ? strcat(perms, "w") : strcat(perms, "-");
                (s.st_mode & S_IXUSR) ? strcat(perms, "x") : strcat(perms, "-");
                (s.st_mode & S_IRGRP) ? strcat(perms, "r") : strcat(perms, "-");
                (s.st_mode & S_IWGRP) ? strcat(perms, "w") : strcat(perms, "-");
                (s.st_mode & S_IXGRP) ? strcat(perms, "x") : strcat(perms, "-");
                (s.st_mode & S_IROTH) ? strcat(perms, "r") : strcat(perms, "-");
                (s.st_mode & S_IWOTH) ? strcat(perms, "w") : strcat(perms, "-");
                (s.st_mode & S_IXOTH) ? strcat(perms, "x") : strcat(perms, "-");

                // Getting last modified time and date
                // Date/Time format is "Month Date Hour:Minute"
                strftime(time, 30, "%b %d %H:%M", localtime(&(s.st_mtime)));

                printf("%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, d -> d_name);
                printf("\n");
            }
        }
        if(num_input_dirs >= 2 && idx != total_arguments - 1){
            printf("\n");
        }
    }    
}

// This function prints only ls -a
void ls_a(char *dir){

    struct dirent *d;
    DIR *reader = opendir(dir);

    // Checking if directory
    if(reader == NULL){

        // Checking if file
        // This is for ls -a <file_name>
        if(access(dir, F_OK) == 0){
            printf("%s ", get_file_name(dir));
        }
        else{
            perror("ls error");
        }

    }
    else{

        // Traversing through directory and printing all files
        while((d = readdir(reader)) != NULL){ 
            printf("%s ", d -> d_name);   
        }
        printf("\n");

        if(num_input_dirs >= 2 && idx != total_arguments - 1){
            printf("\n");
        }
    }
}

// This function prints only ls -a -l or any possible permutation of these flags
void ls_al(char *dir){
    
    struct dirent *d;
    DIR *reader = opendir(dir);
    
    struct dirent *d_temp;
    DIR *reader_temp = opendir(dir);

    // Checking if directory
    if(reader == NULL){ 

        // Checking if file
        // This is for ls -al <file_name>
        if(access(dir, F_OK) == 0){

            char perms[12] = "", time[30];
            
            struct stat s;
            lstat(dir, &s);

            struct passwd *pw;
            pw = getpwuid(s.st_uid);

            struct group *gr;
            gr = getgrgid(s.st_gid);
            
            // Getting file permissions
            S_ISDIR(s.st_mode) ? strcat(perms, "d") : strcat(perms, "-");
            (s.st_mode & S_IRUSR) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWUSR) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXUSR) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IRGRP) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWGRP) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXGRP) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IROTH) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWOTH) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXOTH) ? strcat(perms, "x") : strcat(perms, "-");

            // Getting last modified time
            // Date/Time format is "Month Date Hour:Minute"
            strftime(time, 30, "%b %d %H:%M", localtime(&(s.st_mtime)));
               
            if(first_ls_l_print == 0){
                printf("%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, get_file_name(dir)); 
                first_ls_l_print = 1;
            }
            else{
                printf("\n%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, get_file_name(dir)); 
            }
        }
        else{
            perror("ls error");
        }
    }
    else{

        int size = 0;

        // Traversing through all files of directory to find total block size of all files
        while((d_temp = readdir(reader_temp)) != NULL){
            
            // Getting file path
            char *file_path = (char *)malloc(DIR_SIZE * sizeof(char));
            strcpy(file_path, dir);
            strcat(file_path, "/");
            strcat(file_path, d_temp -> d_name);

            struct stat s;
            lstat(file_path, &s);

            size += s.st_blocks;
        }
        
        printf("total %d\n", size);

        // Traversing through all files of directory to print file perms and details
        while((d = readdir(reader)) != NULL){
            
            // Getting file path
            char *file_path = (char *)malloc(DIR_SIZE * sizeof(char));
            strcpy(file_path, dir);
            strcat(file_path, "/");
            strcat(file_path, d -> d_name);

            char perms[12] = "", time[30];

            struct stat s;
            lstat(file_path, &s);

            struct passwd *pw;
            pw = getpwuid(s.st_uid);

            struct group *gr;
            gr = getgrgid(s.st_gid);

            // Getting file perms
            S_ISDIR(s.st_mode) ? strcat(perms, "d") : strcat(perms, "-");
            (s.st_mode & S_IRUSR) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWUSR) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXUSR) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IRGRP) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWGRP) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXGRP) ? strcat(perms, "x") : strcat(perms, "-");
            (s.st_mode & S_IROTH) ? strcat(perms, "r") : strcat(perms, "-");
            (s.st_mode & S_IWOTH) ? strcat(perms, "w") : strcat(perms, "-");
            (s.st_mode & S_IXOTH) ? strcat(perms, "x") : strcat(perms, "-");

            // Getting last modified time
            // Date/Time format is "Month Date Hour:Minute"
            strftime(time, 30, "%b %d %H:%M", localtime(&(s.st_mtime)));

            printf("%10s %3ld %8s %8s %6ld %12s %s", perms, s.st_nlink, pw->pw_name, gr -> gr_name, s.st_size, time, d -> d_name);
            printf("\n"); 
        }
        if(num_input_dirs >= 2 && idx != total_arguments - 1){
            printf("\n");
        }
    }
}

// Driver Function
void ls(){

    // This is for "ls"
    if(total_arguments == 1){
        normal_ls(".");
        return;
    }

    // Initialising all the flags
    setflags();
    first_ls_l_print = 0;

    if(non_flag_ls >= 1){
        idx = 1;

        for(idx=1; idx < total_arguments; idx++){

            char *INPUT_DIR = ".";
            if(command[idx][0] != '-'){
                
                INPUT_DIR = get_abs_path(command[idx]);

                struct dirent *d;
                DIR *reader = opendir(INPUT_DIR);
                if(reader == NULL){
                    
                    if(access(INPUT_DIR, F_OK) == 0){

                        if(flags[0]==0 && flags[1]==0){
                            normal_ls(INPUT_DIR);
                        }
                        else if(flags[0]==1 && flags[1]==0){
                            ls_a(INPUT_DIR);
                        }
                        else if(flags[0]==0 && flags[1]==1){
                            ls_l(INPUT_DIR);
                        }
                        else{
                            ls_al(INPUT_DIR);
                        }
                    }
                    else{
                        perror("ls error");
                    }
                }

                free(INPUT_DIR);
            }
        }
        
        if(non_flag_ls - num_input_dirs >= 1 && num_input_dirs >= 1){
            printf("\n\n");
        }
        idx = 1;
        
        for(idx=1; idx < total_arguments; idx++){

            char *INPUT_DIR = ".";
            if(command[idx][0] != '-'){
                
                INPUT_DIR = get_abs_path(command[idx]);
                
                struct dirent *d;
                DIR *reader = opendir(INPUT_DIR);
                if(reader == NULL){

                    if(access(INPUT_DIR, F_OK) != 0){
                        perror("ls error");
                    }

                }
                else{

                    if(num_input_dirs >= 2 || (non_flag_ls - num_input_dirs >= 1)){
                        printf("%s:\n", INPUT_DIR);
                    }

                    if(flags[0]==0 && flags[1]==0){
                        normal_ls(INPUT_DIR);
                    }
                    else if(flags[0]==1 && flags[1]==0){
                        ls_a(INPUT_DIR);
                    }
                    else if(flags[0]==0 && flags[1]==1){
                        ls_l(INPUT_DIR);
                    }
                    else{
                        ls_al(INPUT_DIR);
                    }
                }
                free(INPUT_DIR);
            }
        }
    }
    else{
        
        if(flags[0] == 0 && flags[1] == 0){
            normal_ls(".");
        }
        else if(flags[0] == 1 && flags[1] == 0){
            ls_a(".");
        }
        else if(flags[0] == 0 && flags[1] == 1){
            ls_l(".");
        }
        else{
            ls_al(".");
        }
    }
}