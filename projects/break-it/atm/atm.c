#include "atm.h"
#include "../ports.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <arpa/inet.h>
#include "../crypto/crypto.h"

#define KEY_SIZE 32
#define IV_SIZE 16
static const int test = 0;
unsigned char *key = NULL;
unsigned char *iv = NULL;

ATM* atm_create()
{
    
    ATM *atm = (ATM*) malloc(sizeof(ATM));
    if(atm == NULL)
    {
        perror("Could not allocate ATM");
        exit(1);
    }

    // Set up the network state
    atm->sockfd=socket(AF_INET,SOCK_DGRAM,0);

    atm->user = NULL;

    bzero(&atm->rtr_addr,sizeof(atm->rtr_addr));
    atm->rtr_addr.sin_family = AF_INET;
    atm->rtr_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    atm->rtr_addr.sin_port=htons(ROUTER_PORT);

    bzero(&atm->atm_addr, sizeof(atm->atm_addr));
    atm->atm_addr.sin_family = AF_INET;
    atm->atm_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    atm->atm_addr.sin_port = htons(ATM_PORT);
    bind(atm->sockfd,(struct sockaddr *)&atm->atm_addr,sizeof(atm->atm_addr));

    // Set up the protocol state
    // TODO set up more, as needed

    return atm;
}

void atm_free(ATM *atm)
{
    if(atm != NULL)
    {
        close(atm->sockfd);
        free(atm);
    }
}

ssize_t atm_send(ATM *atm, char *data, size_t data_len)
{
    // Returns the number of bytes sent; negative on error
    unsigned char ciphertext[512];
    int ciphertext_len = encrypt((unsigned char *)data, data_len, key, iv,ciphertext);
    return sendto(atm->sockfd, ciphertext, ciphertext_len, 0,
                  (struct sockaddr*) &atm->rtr_addr, sizeof(atm->rtr_addr));
}

ssize_t atm_recv(ATM *atm, char *data, size_t max_data_len)
{
    unsigned char plaintext[128];
    unsigned char ciphertext[128];
    int bytes_received = recvfrom(atm->sockfd, ciphertext, 128, 0, NULL, NULL);
    int plaintext_len = decrypt(ciphertext, bytes_received, key, iv, plaintext);
    memcpy(data, plaintext, plaintext_len);
    return plaintext_len;
}

static int regex_match(char *string, char *pattern) {
    //int status;
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

char *get_user(ATM *atm){
    return atm->user;
}

static void flushstdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int atm_process_command(ATM *atm, char *command, const char *init_path)
{
    //Get command
    if (command[strlen(command)-1] == '\n') {
        command[strlen(command)-1] = '\0'; //remove newline
    }
    if (key == NULL) {
        FILE *fp = fopen(init_path, "r");
        key = malloc(KEY_SIZE);
        fread((char *)key, 1, KEY_SIZE, fp);
        fclose(fp);        
    }
    if (iv == NULL) {
        FILE *fp = fopen(init_path, "r");
        iv = malloc(IV_SIZE);
        fread((char *)iv, 1, IV_SIZE, fp);
        fclose(fp);
    }
    char *token = strtok(command, " ");
    if (token) {
        // Process commands
        if (strcmp(token, "begin-session") == 0) {
            // Read username and username length
            char* username = strtok(NULL, " ");
            char* extra = strtok(NULL, " ");
            int username_len;
            if (username && !extra && regex_match(username,"^[a-zA-Z]{1,250}$")) {
                username_len = strlen(username);
            } else if (atm->user != NULL) {
                printf("A user is already logged in\n");
                return -1;
            } else {
                // Invalid username entered
                printf("Usage:\tbegin-session <user-name>\n");
                return -1;
            }
            // Construct verify username request
            char *auth_user_request;
            auth_user_request = (char *) malloc(2 + username_len);

            //strcat(auth_user_request, "0");
            auth_user_request[0] = 0;
            //strcat(auth_user_request, username_len_str);
            auth_user_request[1] = username_len;
            //strcat(auth_user_request, username);
            memcpy(auth_user_request+2, username, username_len);

            // Send request to verify username
            if(!test){
                int packet_size = 2+username_len;
                atm_send(atm, auth_user_request, packet_size);
            }

            // Receive and process bank response
            char auth_user_resp[2];
            if(test){
                auth_user_resp[0] = 4; //TEST PURPOSES
                auth_user_resp[1] = 1; //TEST PURPOSES
            }else{
                atm_recv(atm, auth_user_resp, 2); // only expect 2 byte response
            }
            char message_code = auth_user_resp[0];
            char response_code = auth_user_resp[1];
            if (message_code == 4 && response_code == 0) {
                // User is not registered
                printf("No such user\n");
                return -1;

            } else if (message_code == 4 && response_code == 1) {
                //User is registered with bank

                //Check for card file and open if it is there
                FILE *card_file;
                char *card_file_name;
                card_file_name = (char *) malloc(username_len + 6);
                strncpy(card_file_name, username, username_len+1);
                strcat(card_file_name, ".card");
                if (access(card_file_name, F_OK) != -1) {
                    card_file = fopen(card_file_name, "r");
                } else {
                    printf("Unable to access %s's card\n", username);
                }
                unsigned char user_real_pin[33];
                fgets((char*)user_real_pin, 1000, card_file);
                user_real_pin[32] = '\0';
                //Prompt user for PIN
                fputs("PIN? ", stdout);
                fflush(stdout);
                char user_input_pin[6];
                fgets(user_input_pin, 6, stdin);
                if (!strchr(user_input_pin,'\n')) {
                    flushstdin();
                }
                unsigned char* input_hash = SHA256((unsigned char*)user_input_pin, 4, 0);
                // Check if pin is correct
                //printf("%d\n", strncmp((char*)input_hash, (char*)user_real_pin, 32));
                if (regex_match(user_input_pin,"^[0-9]{4}\n") && strncmp((char*)input_hash, (char*)user_real_pin,32) == 0) {
                    atm->user = malloc(strlen(username)+1);
                    strcpy(atm->user, username);
                    printf("Authorized\n");
                } else {
                    char end_session_request[1] = {3};
                    atm_send(atm,end_session_request,1);
                    printf("Not Authorized\n");
                    return -1;
                }

            } else if (message_code == 4 && response_code == 2) {
                //A user is already logged in
                printf("A user is already logged in\n");
                return -1;
            }
            return 1;
        // These commands require authorization
        } else if (strcmp(token, "withdraw") == 0) {
            if(atm->user==NULL){
                printf("No user logged in\n");
                return -1;
            }
            char *val = strtok(NULL, " ");
            char *extra = strtok(NULL, " ");
            if(!val || !regex_match(val, "^[0-9]+$") || extra){
                //no number given or negative
                printf("Usage:\twithdraw <amt>\n");
                return -1;
            }
            unsigned int amt = atoi(val);
            unsigned int network_amt = htonl(amt);

            //Packet construction and stuff
            char *withdraw_request = malloc(sizeof(unsigned int)+1);
            char withdraw_resp[2];
            int withdraw_status = 0;
            withdraw_request[0]=1;
            memcpy(withdraw_request+1, (char *)&network_amt,sizeof(int));
            if(!test){
                atm_send(atm, withdraw_request, 1+sizeof(int));
                withdraw_status = atm_recv(atm, withdraw_resp, 2);
            }else{
                //for testing without running the bank
                withdraw_status = 1;
                withdraw_resp[0] = 5;
                withdraw_resp[1] = 1;
            }
            if(withdraw_status<0||withdraw_resp[0]!=5){
                //failure or wrong response
                printf("Usage:\twithdraw <amt>\n");
                return -1;
            }else if(withdraw_resp[1]==0){
                printf("Insufficient funds\n");
                return -1;
            }else if(withdraw_resp[1]==1){
                printf("$%u dispensed\n", amt);
                return 0;
            }else{
                printf("No user logged in\n");
                return -1;
            }

            
        } else if (strcmp(token, "balance") == 0) {

            //Make sure a user is logged in
            if (atm->user == NULL) {
                printf("No user logged in\n");
                return -1;
            }

            //Make sure inputs are valid
            if (strtok(NULL, " ") != NULL) {
                printf("Usage:\tbalance\n");
                return -1;
            }

            //Construct and send balance request to bank
            char *balance_request = (char*) malloc(1);
            balance_request[0] = 2;
            atm_send(atm, balance_request, 1);

            //Receive and process bank response
            int resp_len = 2+sizeof(unsigned int);
            char *balance_resp = (char *) malloc(resp_len);
            // balance_resp[0] = 6; //TEST PURPOSES
            // balance_resp[1] = 1; // TEST PURPOSES
            // unsigned int balance = 100; //TEST PURPOSES
            // memcpy(balance_resp+2, (unsigned int *)&balance, sizeof(unsigned int)); //TEST PURPOSES
            atm_recv(atm, balance_resp, resp_len);
            if (balance_resp[0] == 6 && balance_resp[1] == 0) {
                printf("No user logged in\n");
            } else if (balance_resp[0] == 6 && balance_resp[1] == 1) {
                    unsigned int balance = ntohl(*(unsigned int *)(balance_resp+2));
                    printf("$%u\n", balance);
            }

        } else if (strcmp(token, "end-session") == 0) {
            if(atm->user == NULL){
                printf("No user logged in\n");
                return -1;
            }
            char end_request[1];
            end_request[0] = 3;
            if(!test){
                atm_send(atm, end_request, 1);
            }
            free(atm->user);
            atm->user = NULL;
            printf("User logged out\n");
        } else{
            printf("Invalid command\n");
            return -1;
        }
    } else { //no command entered
        printf("Invalid command\n");
    }
    return 0;
}   
