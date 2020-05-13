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

/*Using the OpenSSL package in C, encrypt the contents of the
file specified on the command line with AES-256 given the key and IV as
specified. This ciphertext should be written to a binary file
named cipher2.out, though you do not need to read this back in,
unless you want to.  Then, use SHA256 to hash this ciphertext
to a short string, also in C, which should be output as hexadecimal,
not raw binary.  You should have a single file, called encrypt2.c,
that compiles to a program using OpenSSL and takes a filename
as input. Your submission for this task should include:

AES takes a 256-bit (32-byte) key, and a 128-bit (16-byte) IV.
You will use your student ID, right-padded with 0's, as the key
(NOT a password), and a string of 16 0's as the IV. That is, if
your UID is 123456789, then the key string would be
"12345678900000000000000000000000" and the IV "0000000000000000".

- encrypt2.c, containing your C code for encrypting and 
 hashing an input file

When called as `./encrypt2 <filename>`, it should produce:

- cipher2.out, containing the ciphertext produced using AES-256
in CBC mode 
- hash2.txt, containing the SHA256 hash of the AES-256 
 ciphertext */

int main( int argc, char **argv){

	// Parse arguments.
	char *plaintext_filename = argv[1];
	char *cipher_filename = "cipher2.out";
	char *hash_filename = "hash2.txt";

	/* A 256 bit key */
	unsigned char *key = (unsigned char *)"11692847200000000000000000000000";
	/* A 128 bit IV */
    unsigned char *init_vector = (unsigned char *)"0000000000000000";
	

	/* Message to be encrypted */
	int block_size = 128;
    unsigned char *plaintext = (unsigned char *)malloc(sizeof(unsigned char) * block_size);
	int plaintext_fd = open(plaintext_filename, O_RDONLY);

	/* Buffer for ciphertext.*/
    unsigned char ciphertext[256];
	int cipher_fd = open(cipher_filename, O_CREAT | O_WRONLY );
	chmod(cipher_filename, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	
	/* Buffer for hash */
	unsigned char *hash;
	unsigned int hash_len;

	/* Start encryption process and hash each read_bytes encrypted at same time */
	
	EVP_MD_CTX *mdctx;
	EVP_CIPHER_CTX *ctx;
    int ciphertext_len;
	int read_bytes = 0;
	int iter = 0;
	int len;

    /* Create and initialise the contexts for cipher and hash */
    if(!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();
	if((mdctx = EVP_MD_CTX_new()) == NULL)
			handleErrors();
	/*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, init_vector))
		handleErrors();

	/* Initialize digest function with sha256 */
	if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
		handleErrors();

	while(1){
		bzero(plaintext,block_size);
		bzero(ciphertext,256);
		/* Read a block of plaintext */
		read_bytes = read(plaintext_fd,plaintext,block_size);

		/* Encrypt it */

		/* Last block in file */
		if(read_bytes < block_size){ 

			/* Provide the message to be encrypted, and obtain the encrypted output.
			 * EVP_EncryptUpdate can be called multiple times if necessary*/
			if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, read_bytes))
				handleErrors();
			ciphertext_len = len;

			/*Finalise the encryption. Further ciphertext bytes may be written at
			 * this stage*/
			if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
				handleErrors();
			ciphertext_len += len;
			
			/*Provide the message whose digest needs to be calculated. Messages can be divided. */
			if(1 != EVP_DigestUpdate(mdctx, ciphertext, ciphertext_len))
				handleErrors();

			/* Allocate space for checksum */
			if(( hash  = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)
				handleErrors();

			/* Finalize SHA256 hash of last block */
			if(1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len))
				handleErrors();

			/* Write ciphertext produced by AES-256 in CBC mode to cipher2.out */
			write(cipher_fd, ciphertext, ciphertext_len);

			break;
		}
		else{
			/* Provide the message to be encrypted, and obtain the encrypted output.
			 * EVP_EncryptUpdate can be called multiple times if necessary*/
			if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, read_bytes))
				handleErrors();
			ciphertext_len = len;

			/*Provide the message whose digest needs to be calculated. Messages can be divided. */
			if(1 != EVP_DigestUpdate(mdctx, ciphertext, ciphertext_len))
				handleErrors();

			/* Write ciphertext produced by AES-256 in CBC mode to cipher2.out */
			write(cipher_fd, ciphertext, ciphertext_len);
		}

		iter++;
	}

	/* Finish encryption and hashing process */

	/* Write SHA256 hash in hexadecimal form to hash2.txt */
	FILE *hash_file = fopen(hash_filename, "w");
	for(size_t i = 0; i < hash_len ; ++i) {
		fprintf(hash_file, "%02x", hash[i]);
	}
	fprintf(hash_file, "\n");

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
	EVP_MD_CTX_free(mdctx);
	free(plaintext);
	free(hash);
	fclose(hash_file);	
	close(plaintext_fd);
	close(cipher_fd);
	
	return 0;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

