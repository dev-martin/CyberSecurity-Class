#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>

/*
 * A union is a data type in C that overlays the fields in the same
 * section of memory. In this case, we've mapped a 32-bit unsigned int
 * into the same memory as an array of 4 8-bit unsigned characters.
 * This is a convenient way to serialize and deserialize data, at least
 * for simple data types.
 */
typedef union {
   uint32_t val;
   unsigned char bytes[4];
} block_t;

/* This assumes the nibble is the smaller half, ie the byte is 0x0X */
unsigned char sbox(unsigned char input) {
   unsigned char output;
   /* Sanitize our input */
   unsigned char nibble = input & 0x0F;

   /* Your lookup table goes here */
   output = nibble;

   return output;
}

uint32_t pbox(uint32_t input) {
   uint32_t output = 0;

   /* Your translation goes here */
   for ( int i = 0 ; i < 32 ; ++i ) {
      int j = i;
      if ( input & (1 << (31-i)) ) {
         output |= (1 << (31-j));
      }
   }

   return output;
}

int main(int argc, char** argv) {
   block_t input;
   block_t output;
   if ( argc > 1 ) {
      /* Parse the first argument as a 4-byte hexadecimal number (8 characters) */
      uint32_t v;
      if ( 0 < sscanf(argv[1],"%8x",&v) ) {
         /* The value we read in is in host byte order, but we want to ensure that
            it's in big-endian order (ie, network byte order) so that the bytes
            line up correctly. */
         input.val = htonl(v);

         /* Now let's see what we have, on STDERR (you may delete this) */
         fprintf(stderr,"%u\n", input.val);
         fprintf(stderr,"%x %x %x %x\n", input.bytes[0], input.bytes[1], input.bytes[2], input.bytes[3]);

         /* Split the input bytes, and pass them to the sboxes. */

         /* Pass the sbox output to the pbox. */

         /* Convert back to host byte order. */
         printf("%08x\n", ntohl(output.val));
      }
   }
   return 0;
}

