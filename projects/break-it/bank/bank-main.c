/* * The main program for the Bank.
 *
 * You are free to change this as necessary.
 */

#include <string.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include "bank.h"
#include "ports.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_MSG_SIZE (sizeof("create-user")-1)+1+250+1+4+1+(sizeof("4294967295")-1)

static const char prompt[] = "BANK: ";

int main(int argc, char **argv)
{
   if ( open(argv[1],O_RDONLY) == -1 ) {
        printf("Error opening bank intialization file\n");
        return 64;
    }
   int n;
   char sendline[MAX_MSG_SIZE+1]; //+1 for null terminator
   char recvline[1000];

   Bank *bank = bank_create();
   HashTable *ht = hash_table_create(10);

   printf("%s", prompt);
   fflush(stdout);

   while(1)
   {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        FD_SET(bank->sockfd, &fds);
        select(bank->sockfd+1, &fds, NULL, NULL, NULL);
        
       if(FD_ISSET(0, &fds))
       {
           fgets(sendline, MAX_MSG_SIZE+1,stdin);
           if (!strchr(sendline,'\n')) { //if there is input in buffeer over MAX_MSG_SIZE
                while((getchar()) != '\n'); //discard input over MAX_MSG_SIZE
           }
           bank_process_local_command(bank, sendline, strlen(sendline), ht);
           printf("\n"); //to abide by formatting
           printf("%s", prompt);
           fflush(stdout);
       }
       else if(FD_ISSET(bank->sockfd, &fds))
       {
           n = bank_recv(bank, recvline, 1000, argv[1]);
           bank_process_remote_command(bank, recvline, n, ht);
       }
   }

   return EXIT_SUCCESS;
}
