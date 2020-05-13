#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

void handleErrors(void);

int main( int argc, char **argv){

	// Parse arguments.
	char *cipher_filename = argv[1];
	char *decrypted_filename  = "decrypted2.out";

	/* A 256 bit key */
    unsigned char *key = (unsigned char *)"11692847200000000000000000000000";
	/* A 128 bit IV */
    unsigned char *init_vector = (unsigned char *)"0000000000000000";
	

	/* Message to be decrypted */
	int block_size = 128;
    unsigned char *ciphertext = (unsigned char *)malloc(sizeof(unsigned char) * block_size);
	int cipher_fd = open(cipher_filename, O_RDONLY);

	/* Buffer for derypted text.*/
    unsigned char *decryptedtext = (unsigned char *)malloc(sizeof(unsigned char) * block_size);
	int decrypted_fd = open(decrypted_filename, O_CREAT | O_WRONLY | O_NONBLOCK);
	chmod(decrypted_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	

	/* Start decryption process */
	
	EVP_CIPHER_CTX *ctx;
    int decryptedtext_len;
	int read_bytes = 0;
	int iter = 0;
	int len;

    /* Create and initialise the contexts for cipher */
    if(!(ctx = EVP_CIPHER_CTX_new()))

	/*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, init_vector))
		handleErrors();

	while(1){
		bzero(ciphertext,block_size);
		bzero(decryptedtext,128);
		/* Read a block of plaintext */
		read_bytes = read(cipher_fd,ciphertext,block_size);

		/* Encrypt it */

		/* Last block in file */
		if(read_bytes < block_size){ 

			/*
			 * Provide the message to be decrypted, and obtain the plaintext output.
			 * EVP_DecryptUpdate can be called multiple times if necessary.
			 */
			if(1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, read_bytes))
				handleErrors();
			decryptedtext_len = len;

			/*
			 * Finalise the decryption. Further plaintext bytes may be written at
			 * this stage.
			 */
			if(1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len))
				handleErrors();
			decryptedtext_len += len;


			/* Write decryptedtext  produced by AES-256 in CBC mode to decrypted2.out */
			write(cipher_fd, decryptedtext , decryptedtext_len );

			break;
		}
		else{
			/*
			 * Provide the message to be decrypted, and obtain the plaintext output.
			 * EVP_DecryptUpdate can be called multiple times if necessary.
			 */
			if(1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, read_bytes))
				handleErrors();
			decryptedtext_len = len;

			/* Write decryptedtext  produced by AES-256 in CBC mode to decrypted2.out */
			write(cipher_fd, decryptedtext , decryptedtext_len );
		}

		iter++;
	}

	/* Finish dencryption */

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
	free(ciphertext);
	close(decrypted_fd);
	close(cipher_fd);
	
	return 0;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

