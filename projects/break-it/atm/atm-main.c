/* 
 * The main program for the ATM.
 *
 * You are free to change this as necessary.
 */

#include "atm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define EXIT_NO_INIT_FILE 64
#define MAX_MSG_SIZE (sizeof("begin-session")-1)+1+250

static const char prompt[] = "ATM: ";

int main(int argc, char **argv) {

    // Check for correct number of arguments
    if (argc != 2) {
        printf("Error opening ATM initialization file\n");
        return EXIT_NO_INIT_FILE;
    }

    if (access(argv[1], F_OK) == -1) {
        printf("Error opening ATM initialization file\n");
        return EXIT_NO_INIT_FILE;
    }
    
    char user_input[MAX_MSG_SIZE+1];
    ATM *atm = atm_create();
    printf("%s", prompt);
    fflush(stdout);

    while (fgets(user_input, MAX_MSG_SIZE+1,stdin) != NULL)
    {
        if (!strchr(user_input,'\n')) { //if there is input in the buffer over MAX_MSG_SIZE
            while ((getchar()) != '\n'); //discard input over MAX_MSG_SIZE
        }
        atm_process_command(atm, user_input, argv[1]);
        printf("\n"); //to abide by formatting
 
        char *name = get_user(atm);
        if(name==NULL){
            //No user logged in
            printf("%s", prompt);
        }else{
            printf("ATM (%s): ",name);
        }
        fflush(stdout);
    }
	return EXIT_SUCCESS;
}
