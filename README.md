# Introduction

This is a Linux shell implemented in C.

# Running the Shell

1. cd into the directory containing all the source files
2. run the command `make`
3. run `./a.out`

# Functionality

- **ls**
    - This commands Lists all the files and directories.
    - Using it with flags gives additional information such as hidden files, last edit date etc.
    - Flags implemented are `-l` and `-a`
    - General Usage: `ls [flags] [Directory[/File_name]]`
    - Usage Examples:
        - `ls`
        - `ls -a`
        - `ls -l`
        - `ls -a -l`
        - `ls -al / ls -la`
        - `ls .`
        - `ls ..`
        - `ls temp.c`
        - `ls [-flags] temp/test.txt`

- **echo**
    - Prints the string specified after removing extra spaces and tabs
    - General Usage; `echo <string>`
    - Usage Exampls:
        - `echo "hey"`
        - `echo "hey 'my 'name "is bond,    James       Bondd!`

- **pwd**
    - Gives the full path of the present working directory
    - Usage: `pwd`
     
- **history**
    - Gives a list of the latest commands entered in the shell.
    - User can specify a number upto 20.
    - If no number is specified, last 10 commands are printed.
    - General Usage: `history [Number]`
    - Usage Examples:
        - `history`
        - `history 5`
        - `history 10`
        - `history 20`
- **cd**
    - Current working directory changes to the input directory.
    - General Usage: `cd [Directory]`
    - Usage Examples:
        - `cd`
        - `cd .`
        - `cd ..`
        - `cd ~`
        - `cd -`
        - `cd temp`
        - `cd temp/temp1`
    
- **Foreground Process**
    - Runs the command inputted
    - General Usage: `<command> [flags]`
    - Usage Examples:
        - `gedit`
        - `firefox`
        - `clear`
        - `clear -X`

- **Foreground Process**
    - Runs the inputted command in the background
    - General Usage: `<command> [flags] &`
    - Usage Examples:
        - `gedit &`
        - `firefox &`
        - `sleep 10 &`

- **Multiple Commands**
    - User can run multiple commands in a single input.
    - Each command should be seperated by a semi-colon.
    - General Usage: `<command_1> ; <command_2> [; [command_n]]`
    - Usage Examples:
        - `ls; sleep 5; firefox &`

- **Process Information**
    - Gives information about the process specified by pid
    - If no pid is given, then information about currently running process is given.
    - General usage: `pinfo [pid]`
    - Usage Examples:
        - `pinfo`
        - `pinfo 6921`

- **Repeat**
    - Repeats the inputted command specified number of times
    - General Usage: `repeat <Num_Repeat> <command>`
    - Usage Examples:
        - `repeat 2 echo hello`
        - `repeat 3 sleep 5`


- **Input/Output Redirection**
    - User can use contents of a file as input.
    - User can redirect output of a command into a file.
    - `<` is used to specidy input file
    - `>` is used to specify output file
    - `>>` is used to specify to concatenate to output file.
    - General Usage: `<command> < <input_file> > <output_file>`
    - Usage Examples:
        - `echo Hello > 1.txt`
        - `cat < 1.txt`
        - `sort < test.txt >> sorted.txt`

- **Pipelining**
    - Ouput of one command can be provided as input of other command
    - General Usage: `<command_1> | <command_2>`
    - Usage Examples: 
        - `cat 1.txt | wc`
        - `cat 2.txt | head -7 | tail -5`
        - `cat < 3.txt | wc -l | lines.txt`

- **jobs**
    - This command gives basic information about all background jobs
    - Has 2 flags: `-r` and `-l`
    - `-r` gives information about running background jobs
    - `-s` gives information about sleeping background jobs
    - General Usage: `jobs [-rs]`
    - Usage Examples:
        - `jobs`
        - `jobs -r`
        - `jobs -s`

- **sig**
    - This command sends signal to a background job specified by job number
    - General Usage: `sig <Job_Number> <Signal_Number>`
    - Usage Examples:
        - `sig 2 9`
        - `sig 1 12`

- **fg**
    - `fg` brings the running or stopped background job corresponding to ​j​ob number​​ to the foreground, and changes its state to ​r​running.​
    - The shell throws an error if no job with the given job number exists.
    - General Usage: `fg <Job_Number>`
    - Usage Example:
        - `fg 1`

- **bg**
    - `bg` changes the state of a stopped background job to running (in the background).
    - The shell throws an error if no background job corresponding to the given job number exists
    - The shell does nothing if the job is already running in the background.
    - General Usage: `bg <Job_Number>`
    - Usage Example:
        -`bg 1`

- **Signal Handling**
    - `CTRL-Z`
        - Pushes any currently running foreground job into the background, and changes its state from running to stopped. 
        - This havs no effect on the shell if there is no foreground process running.
    - `CTRL-C`
        - This interrupts any currently running foreground job by sending it the ​S​SIGINT​​ signal. 
        - This has no effect on the shell if there is no foreground process running.
    - `CTRL-D`
        - Logs the user out of the shell.
        - Has no effect on the actual shell.

- **Replay**
    - command executes a particular command in fixed time interval for a certain period.
    - General Usage: `replay -command <command> -interval <Interval_Number> -period <Period_Number>`
    - Usage Examples:
        - `replay -command echo "hi" -interval 3 -period 6`
        - `replay -command sleep 2 -interval 2 -period 11`

- **exit**
    - This command exits the shell.

----

# File Structure

- `main.c` contains code to initiate the shell and calls functions to set it up.
- `utils.c` contains inplementation of the driver functions that help initiate and run the shell
- `headers.h` contains all the headers required by the shell.
- Each function has a file corrsponding to it.
- `<command>.c` contains code to implement `<command>`
- For example: 
    - `ls.c` contains code to implement `ls`