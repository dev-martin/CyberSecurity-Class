#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/rand.h>

int main(int argc, char *argv[]){
    char *atmname;
    char *bankname;
    unsigned char key[32];
    unsigned char iv[16];
    FILE * atmFile;
    FILE * bankFile;
    int len;
    if(argc != 2){
        //returns 62 if more or less than one argument is given
        printf("Usage:\tinit <filename>\n");
        return 62;
    }
    //sets the names of the files to input.atm and input.bank
    //allocates the length of the file name + 5 for .atm (includes null terminator) and +6 for .bank (includes null terminator)
    len = strlen(argv[1]);
    atmname = (char*) malloc(len+5);
    bankname = (char*)malloc(len+6);
    if(atmname==NULL||bankname==NULL){
        //malloc failure
        printf("Error creating initialization files");
        return 64;
    }
    strncpy(atmname, argv[1], len+1);
    strcat(atmname, ".atm");

    strncpy(bankname, argv[1], len+1);
    strcat(bankname, ".bank");


    if(access(atmname, F_OK) != -1 || access(bankname, F_OK) != -1){
        //returns 63 if one of the files exists
        printf("Error:\tone of the files already exists\n");
        return 63;
    }
    atmFile = fopen(atmname, "w");
    bankFile = fopen(bankname, "w");
    free(atmname);
    free(bankname);
    if(!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))){
        //RAND_bytes returned 0 which is a failure
        printf("Error creating initialization files");
        return 64;
    }
    fwrite((char *)key, 1, sizeof(key), atmFile);
    fwrite((char *)key, 1, sizeof(key), bankFile);
    //fwrite("testinghelloman1", 1, sizeof(key), atmFile);
    //fwrite("testinghelloman1", 1, sizeof(key), bankFile);
    fwrite((char *)iv, 1, sizeof(iv), atmFile);
    fwrite((char *)iv, 1, sizeof(iv), bankFile);
    //fwrite("testinghelloman1", 1, sizeof(iv), atmFile);
    //fwrite("testinghelloman1", 1, sizeof(iv), bankFile);
    fclose(atmFile);
    fclose(bankFile);
    printf("Successfully initialized bank state\n");
    return 0;
}
