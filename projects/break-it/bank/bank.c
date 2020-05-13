#include "bank.h"
#include "ports.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <limits.h>
#include <errno.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include "../crypto/crypto.h"

#define KEY_SIZE 32
#define IV_SIZE 16
char *curr_atm_user = NULL; //to keep track of current atm session
unsigned char *key = NULL;
unsigned char *iv = NULL;

Bank* bank_create()
{
    Bank *bank = (Bank*) malloc(sizeof(Bank));
    if(bank == NULL)
    {
        perror("Could not allocate Bank");
        exit(1);
    }

    // Set up the network state
    bank->sockfd=socket(AF_INET,SOCK_DGRAM,0);

    bzero(&bank->rtr_addr,sizeof(bank->rtr_addr));
    bank->rtr_addr.sin_family = AF_INET;
    bank->rtr_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bank->rtr_addr.sin_port=htons(ROUTER_PORT);

    bzero(&bank->bank_addr, sizeof(bank->bank_addr));
    bank->bank_addr.sin_family = AF_INET;
    bank->bank_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bank->bank_addr.sin_port = htons(BANK_PORT);
    bind(bank->sockfd,(struct sockaddr *)&bank->bank_addr,sizeof(bank->bank_addr));

    // Set up the protocol state
    // TODO set up more, as needed

    return bank;
}

void bank_free(Bank *bank)
{
    if(bank != NULL)
    {
        close(bank->sockfd);
        free(bank);
    }
}

ssize_t bank_send(Bank *bank, char *data, size_t data_len)
{
    // Returns the number of bytes sent; negative on error
    unsigned char ciphertext[128];
    int ciphertext_len = encrypt((unsigned char *)data, data_len, key, iv, ciphertext);
    return sendto(bank->sockfd, ciphertext, ciphertext_len, 0,
                  (struct sockaddr*) &bank->rtr_addr, sizeof(bank->rtr_addr));
}

ssize_t bank_recv(Bank *bank, char *data, size_t max_data_len, const char *init_path)
{
    if (key == NULL) {
        FILE *fp = fopen(init_path, "r");
        key = malloc(KEY_SIZE);
        fread((char *)key, 1, KEY_SIZE, fp);
        fclose(fp);        
    }
    if (iv == NULL) {
        FILE *fp = fopen(init_path,"r");
        iv = malloc(IV_SIZE);
        fread((char *)iv, 1, IV_SIZE, fp);
        fclose(fp);
    }
    // Returns the number of bytes received; negative on error
    unsigned char plaintext[512];
    unsigned char ciphertext[512];
    int bytes_received = recvfrom(bank->sockfd, ciphertext, 512, 0, NULL, NULL);
    int plaintext_len = decrypt(ciphertext, bytes_received, key, iv, plaintext);
    memcpy(data, plaintext, plaintext_len);
    return plaintext_len;
}

//called after call to regex_match to make sure is integer
static int int_size_check(char *integer_string, unsigned int integer, unsigned int amount_to_add) {
    int success = 0;
    if (integer_string) {
        if (strtol(integer_string,NULL,10) <= UINT_MAX && errno != ERANGE) {
            success = 1;
        }
    } else {
        unsigned long new_val = ((unsigned long)integer)+amount_to_add;
        if (new_val <= UINT_MAX) {
            success = 1;
        }
    }
    return success;
}

static int regex_match(char *string, char *pattern) {
    regex_t re;
    int success = 0;

    if (!regcomp(&re, pattern,REG_EXTENDED|REG_NOSUB)) {
        if (!regexec(&re, string, 0, NULL, 0)) {
            success = 1;
        }
        regfree(&re);
    }
    return success;
}

static void handle_balance(char *username, HashTable *ht) {
   unsigned int *balance = hash_table_find(ht,username);
    if (balance) { 
        printf("$%u\n",*balance);
    } else {
        printf("No such user\n");
    }
}

static void handle_deposit(char *username, unsigned int deposit_amount, HashTable *ht) {
   unsigned int *balance = hash_table_find(ht,username);
    if (balance) {
        if (int_size_check(NULL, *balance, deposit_amount)) { 
            *balance += deposit_amount;
            printf("$%u added to %s's account\n",deposit_amount,username);
        } else {
            printf("Too rich for this program\n");
        }
    } else {
        printf("No such user\n");
    }
}

static void handle_create_user(char *username, char *pin, unsigned int balance, HashTable *ht) {
    if (hash_table_find(ht, username) == NULL) { //user doesn't exist yet

        char *path_buf = malloc(sizeof(char)*(2+strlen(username)+5+1));
        memcpy(path_buf,"./",2);
        strcpy(path_buf+2,username); //ok since validation done beforehand
        memcpy(path_buf+2+strlen(username),".card",5);
        path_buf[2+strlen(username)+5] = '\0';

        FILE *fp;
        if ((fp = fopen(path_buf,"w+")) != NULL) {
            //don't know what to put in card file other than pin
            unsigned char* pin_hash = SHA256((unsigned char*)pin, strlen(pin), 0);

            fwrite(pin_hash,sizeof(char),strlen((char*)pin_hash),fp);
            //TODO: Make sure all data is written

            // next two lines only done as to not have to change 
            // a crapton of hashtable and list definitions from 
            // hashtable implementation even though storing primitive
            unsigned int *balance_val = malloc(sizeof(unsigned int));
            *balance_val = balance;
            char *heap_username = malloc((strlen(username)+1)*sizeof(char));
            memcpy(heap_username,username,strlen(username));
            heap_username[strlen(username)] = '\0';
            hash_table_add(ht, heap_username, balance_val);
            printf("Created user %s\n",username);
            fclose(fp);
        } else {
            printf("Error creating card file for user %s\n",username);
        }
        free(path_buf);
        
    } else {
        printf("Error:\tuser %s already exists\n",username);
    }
}

void bank_process_local_command(Bank *bank, char *command, size_t len, HashTable *ht)
{
    if (command[strlen(command)-1] == '\n') {
        command[strlen(command)-1] = '\0'; //remove newline
    }
    char *token = strtok(command, " "); //get command
    if (token) {
        if (strcmp(token, "create-user") == 0) {
            char *username = strtok(NULL, " ");
            char *pin = strtok(NULL," ");
            char *initial_balance = strtok(NULL," ");
            char *extra = strtok(NULL, " ");
            if (username && pin && initial_balance && !extra && regex_match(username,"^[a-zA-Z]{1,250}$") 
                    && regex_match(pin, "^[0-9]{4}$") && regex_match(initial_balance, "^[0-9]+$")
                    && int_size_check(initial_balance,0,0)) {
                handle_create_user(username, pin, atoi(initial_balance), ht); //atoi ok because error checking already done
            } else {
                printf("Usage:\tcreate-user <user-name> <pin> <balance>\n");
            }
        } else if (strcmp(token,"balance") == 0) {
            char *username = strtok(NULL, " ");
            char *extra = strtok(NULL," ");
            if (username && !extra && regex_match(username,"^[a-zA-Z]{1,250}$")) { 
                handle_balance(username,ht);
               
            } else {
                printf("Usage:\tbalance <user-name>\n");
            }
        } else if (strcmp(token,"deposit") == 0) {
            char *username = strtok(NULL, " ");
            char *amount = strtok(NULL, " ");
            char *extra = strtok(NULL, " ");
            if (username && amount && !extra && regex_match(username, "^[a-zA-Z]{1,250}$") && regex_match(amount,"^[0-9]+$") && int_size_check(amount, 0, 0)) {
                handle_deposit(username,atoi(amount),ht);
            } else {
                printf("Usage:\tdeposit <user-name> <amt>\n");
            }
        } else {
            printf("Invalid command\n");
        }
    } else { //no command entered
        printf("Invalid command\n");
    }

}

void bank_process_remote_command(Bank *bank, char *command, size_t len, HashTable *ht)
{
    char response[1000];
    int response_size = 0;
    switch(command[0]) {
        case 0: //begin-session-request
            response[0] = 4;
            if (curr_atm_user == NULL) {
                unsigned char username_size = command[1];
                char *username = malloc((username_size+1)*sizeof(char));
                memcpy(username, command+2, username_size);
                username[username_size] = '\0';
                if (hash_table_find(ht, username) == NULL) {
                    //user isn't registered with bank
                    response[1] = 0;
                    free(username);
                } else {
                    curr_atm_user = username;
                    response[1] = 1;
                }
            } else {
                response[1] = 2; //another user already logged in
            }
            response_size = 2;
            break;
        case 1: //withdraw-request
            response[0] = 5;
            if (curr_atm_user != NULL) {
                unsigned int amount_requested = ntohl(*(unsigned int *)(command+1));
                unsigned int *curr_balance = hash_table_find(ht,curr_atm_user);
                if (amount_requested <= *curr_balance) {
                    *curr_balance -= amount_requested;
                    response[1] = 1; //sufficient funds
                } else {
                    response[1] = 0; //insufficient funds
                }
            } else {
                response[1] = 2; //no user is logged in
            }
            response_size = 2;
            break;
        case 2: //balance-request
            response[0] = 6;
            if (curr_atm_user != NULL) { //user is logged in
                response[1] = 1;
                unsigned int balance = htonl(*(unsigned int *)hash_table_find(ht,curr_atm_user)); //should never be null since check done in begin-session-response
                memcpy(response+2,(unsigned char *)&balance,sizeof(unsigned int));
                response_size = 2+sizeof(unsigned int);
            } else {
                response[1] = 0;
                response_size = 2;
            }
            break;
        case 3: //end-session-request (no response necessary)
            free(curr_atm_user);
            curr_atm_user = NULL;
            break;
        default:
            //printf("Should never get here\n");
            break;
    }
    if (response_size != 0) {
        bank_send(bank,response,response_size);
    }
	
}
