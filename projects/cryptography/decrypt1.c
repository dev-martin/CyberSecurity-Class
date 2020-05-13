/*As a recommendation, you might consider using keyword1.txt and
cipher1.txt as inputs to your decryption program, which would
then produce message1.txt.*/

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

char *read_file(char *filename, int len);
int write_file(char *message);
char *shift_letters(char *cipher, char *key);

int main( int argc, char **argv){

	// Parse arguments.
	char *keyword_filename, *cipher_filename;
	char *key, *cipher, *msg;
	keyword_filename  = argv[1];
	cipher_filename   = argv[2];
    
	key = read_file(keyword_filename, 4);
	cipher = read_file(cipher_filename, 1500);
	
	msg = shift_letters(cipher,key);

	write_file(msg);

	free(key);
	free(msg);
	free(cipher);
}

char *shift_letters(char *cipher, char *key){
	char *msg = (char *)malloc(sizeof(char) * (strlen(cipher) + 1));	
	bzero(msg,strlen(cipher));

	// Shift each ith[0,len(cipher)] position of the ciphertext plus the jth[0 , len(key)] position of key.
	int j = 0;
	int shift = 0;
	int cipher_num = 0;
	for(int i = 0; i < strlen(cipher); i++){
		shift = key[j] - 97; // To make it with respect to 'a'
		cipher_num = (int)cipher[i];
		if( cipher_num-shift >= 97){
			msg[i] = cipher[i] -  shift;	
		}
		// If shit goes below a (97) 
		else {
			msg[i] = cipher[i] -  shift +  26;
		}

		if (j==3){
			j = 0;
		}
		else{
		 	j++;
		}
	}
	return msg;
}

char *read_file(char *filename, int len){
	FILE *file = fopen(filename,"r");
	char *content = (char *)malloc(sizeof(char) * (len + 1));
	char c;
	int index = 0;

	//Start reading file untill we reach end of file (feof() != 0).
	bzero(content,len);
	while( !feof(file) )	
	{
		c = '\0';
		fscanf(file, "%c",&c);		
		// If you read a '\n' means keyword  is ended, stop reading.
		if (c == '\n')
			break;
		else
			content[index] = c;
		index++;
	}
	fclose(file);
	return content;
}

int write_file(char *message){
	//Output to file
	FILE *file = fopen("message1.txt","w");
	fprintf(file, "%s", message);
	fclose(file);
	return 0;
}
