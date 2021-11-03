#include "../headers.h"
#include "../utils/utils.h"
#include "./echo.h"

void echo(){

    // Printing message after removing tabs and spaces
    for(int i=1; i< total_arguments; i++){
        printf("%s ", command[i]);
    }

    printf("\n");
    command_successful = 1;
}