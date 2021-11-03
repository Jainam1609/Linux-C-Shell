#include "./prompt.h"
#include "../headers.h"
#include "../utils/utils.h"


void prompt() {

    // getting hostname and username
    char hostname[MAX_HOSTNAME + 1], username[MAX_USERNAME], pwd[MAX_PWD], final_mssg[BUFFER_SIZE];
    gethostname(hostname, MAX_HOSTNAME + 1);
    getlogin_r(username, MAX_USERNAME);
    getcwd(pwd, MAX_PWD);

    // checking if relative or absolute path need to be printed
    char* is_subtring_of_home = strstr(pwd, HOME_DIR);
    if(is_subtring_of_home == NULL){
        strcpy(final_mssg, pwd);
    }
    else{
        strcpy(final_mssg, "~");
        strcat(final_mssg, pwd + strlen(HOME_DIR));
    }

    // Printing prompt
    printf("<%s@%s:%s>",username, hostname, final_mssg);

}
