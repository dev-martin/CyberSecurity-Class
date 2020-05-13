The overall learning objective of this lab is to get familiar with
the concepts on substitution ciphers and symmetric key and public
key cryptography. You will be getting first-hand experience with
using the OpenSSL commandline tool and library to encrypt/decrypt
messages under different encryption modes, and to construct message
digests with hash functions. The tasks are set up in such a way
that you not only gain experience with the use of these various
mechanisms, but also with their properties. You are encouraged to
think about what these tools are doing, and why you get the results
you get. Some tasks ask you to write up these observations.

You will be using the same baseline docker image that you used for
the previous projects.  We recommend developing on your machine as
normal, and then compiling and running your code within a container:

 * Develop/debug code
 * `docker run -ti --privileged --rm -v "$(pwd):/opt" baseline` to
   start a container in privileged mode (this will be important
   later)
 * `cd /opt` to change to the directory with your files
 * Compile code in the container
 * Run code in the container to verify output
 * `exit` to terminate the container

Also, you will have to read up on the OpenSSL documentation. While
we realize there will be a lot of similarity between your code and
documentation or tutorials that you find, you *must* submit code
written by you, as always.

## Task 1: Cracking the Vigenere cipher

   In class, we only considered the Vigenere Cipher as a generalization
   of the Caesar Cipher. However, as described by Vigenere it can
   be used as a polyalphabetic Caesar-type Cipher. This employs a
   keyword, each letter of which is the shift for successive
   characters of the message.  That is, if we have "abc" as the
   keyword, then the first character of the ciphertext would be the
   first letter of the plaintext plus 0 (the value of "a"), the
   second would be plaintext plus 1, the third would be plaintext
   plus 2, and then the pattern would repeat with the fourth
   character. You can view this as a three-byte block cipher in ECB
   mode, if that helps. Section 5.2.1 of _Security Engineering_ goes
   into detail.

   As we saw with the repeated XOR in class, we can apply frequency
   analysis to individual characters (and even n-grams) encrypted with
   the same substitution. What we need to know is how long the keyword
   is, in order to know which characters are enciphered with the same
   substitution. Anderson discusses this briefly in the book, but you
   might find the Wikipedia pages
   https://en.wikipedia.org/wiki/Kasiski_examination and
   https://en.wikipedia.org/wiki/Index_of_coincidence useful as
   additional references for two particular methods of determining key
   length. You may also find the library's page within the OpenSSL
   Wiki hekpful: https://wiki.openssl.org/index.php/EVP

   Your target ciphertext is contained in cipher1.txt

   You should develop tools (in the language of your choice) to
   perform the Kasiski Examination or Index of Coincidence
   computation, frequency analysis (see crypto-exercises for n-gram
   frequencies for English), and decryption. **Please note that your
   solution does not have to be completely automated.** If you had to
   perform some of the analysis and decryption manually, this should be
   documented in files.txt as part of your submission. Your submission should
   consist of:

   - keyword1.txt, which should include the keyword that you
     determined

   - message1.txt, which should include the plaintext corresponding
     to cipher1.txt

   - files.txt, which should include a list of all the program
     files you used, any build instructions, and
     instructions for calling the program or programs, as well as
     a discussion of any manual decryption steps you needed to take.

   As a recommendation, you might consider using keyword1.txt and
   cipher1.txt as inputs to your decryption program, which would
   then produce message1.txt.

## Task 2: Use OpenSSL for encryption

   OpenSSL (https://www.openssl.org/) is one of the most popular
   packages (at least in C) for performing cryptographic operations.
   It has implementations of all the major cryptosystems, including
   AES, which is what we will be using in this task. It has an API
   that allows it to be used in programs requiring cryptography, as
   well as a number of command-line utilities. OpenSSL documentation
   can be found at https://www.openssl.org/docs/ and
   https://wiki.openssl.org, with
   https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
   and https://wiki.openssl.org/index.php/EVP_Message_Digests being
   the most useful pages for basic usage. Please note that these are
   valuable pages for seeing the correct functions to call, and the
   arguments they accept, but they are only appropriate for small
   messages. As discussed below, your execution environment will be
   significantly constrained, so this code will not work as written.

   AES takes a 256-bit (32-byte) key, and a 128-bit (16-byte) IV.
   You will use your student ID, right-padded with 0's, as the key
   (NOT a password), and a string of 16 0's as the IV. That is, if
   your UID is 123456789, then the key string would be
   "12345678900000000000000000000000" and the IV "0000000000000000".

   As discussed in class, if you want to encrypt more than a single
   block's worth of data, you need to select a streaming mode of
   operation. We will employ CBC (Cipher Block Chain), which will
   automatically pad the input with enough bytes to fill the last
   block.

   Using the OpenSSL package in C, encrypt the contents of the
   file specified on the command line with AES-256 given the key and IV as
   specified. This ciphertext should be written to a binary file
   named cipher2.out, though you do not need to read this back in,
   unless you want to.  Then, use SHA256 to hash this ciphertext
   to a short string, also in C, which should be output as hexadecimal,
   not raw binary.  You should have a single file, called encrypt2.c,
   that compiles to a program using OpenSSL and takes a filename
   as input. Your submission for this task should include:

   - encrypt2.c, containing your C code for encrypting and
     hashing an input file

   When called as `./encrypt2 <filename>`, it should produce:

   - cipher2.out, containing the ciphertext produced using AES-256
     in CBC mode
   - hash2.txt, containing the SHA256 hash of the AES-256
     ciphertext

   To help you out, we've provided files sample2.in, sample2.cipher,
   and sample2.hash. These use the UID "123456789", so you can
   verify that your code is working correctly.  Obviously, you'll
   need to change the UID in your code to match this when testing,
   and to your UID for the submission.

   **When we run your code, we will limit the amount of memory it has
   available to 455kB.** In order to verify that your code meets this
   limit, change the way you run your `encrypt2` program to the
   following (provided to you as a script `constrained_encrypt2`):

   * `sudo cgcreate -a crow:crow -t crow:crow -g memory:test_crypto` to create
      a *control group* owned by user `crow` that will constrain memory; this
      requires a container running in privileged mode
   * `echo 1M > /sys/fs/cgroup/memory/test_crypto/memory.limit_in_bytes` to
      limit the physical memory available to processes in this group
   * `echo 1M > /sys/fs/cgroup/memory/test_crypto/memory.memsw.limit_in_bytes`
      to limit the swap memory available to processes in this group
   * `cgexec -g memory:test_crypto ./encrypt2 <file>` to run the
      program within the constrained group

   This must then be run in a docker container, which the `run.sh` will do:
   
    docker run --privileged --rm -v "$(pwd):/opt" -w /opt baseline ./constrained_encrypt2 <file>

   In `run.sh`, the file is read as an argument to the script,
   so you would just run

    ./run.sh <file>

   The sample text we give you (sample2.in) might work fine with this
   memory limit when your code is written the "wrong" way, so make
   sure you test with a larger input, like the input or output from
   Task 1. We will grade your submission using a larger input file, to
   which you will not have access in advance.

   Please note:  Your submission must be written *entirely* in C
   (excluding the Makefile).  You may *not* use the OpenSSL
   command-line utilities to implement your submission, including
   by calling them using system() or exec().


