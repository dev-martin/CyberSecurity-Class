#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



/*Repeated modular squaring*/
/*-------------------------*/
 
   /*Given a, n, and k, compute b = a^e mod n, where e=2^k*/

   /*Your program should be named square, and will be invoked as*/
   /*./square <a> <n> <k>*/
   /*It should return a line of the form:*/
   /*b = <b>*/
   
   /*That is, given*/
   /*./square 3 55 7*/
   /*your program should return*/
   /*b = 16*/

   /*Note that if you know the factors of n, you can equivalently compute*/
   /*e = 2^k mod phi(n)*/
   /*b = a^e mod n*/
   /*which makes verifying the solution easy.*/

int main(int argc, char *argv[])
{
  long a = atol(argv[1]);
  long n = atol(argv[2]);
  long k = atol(argv[3]);
  long times = powl(2,k);
  long temp = a;
  for(int i = 1; i<times; i++){
  	temp = temp * a % n;
  }
  printf("b = %ld\n",temp);

  return 0;
}
