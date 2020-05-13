#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*Square roots modulo a prime number*/
/*----------------------------------*/
 
   /*Given x and p, find y such that y^2 = x mod p*/
   
   /*Your program (or script) should be named sqrt, and will be invoked as*/
   /*./sqrt <x> <p>*/
   /*It should return a line of the form:*/
   /*sqrt(<x>) = <y> (mod <p>)*/
   
   /*That is, given*/
   /*./sqrt 6 5*/
   /*your program should print either*/
   /*sqrt(6) = 1 (mod 5)*/
   /*or*/
   /*sqrt(6) = 4 (mod 5)*/

int perfect(long number);
int main(int argc, char *argv[])
{
  long x = atol(argv[1]);
  long p = atol(argv[2]);
  long mod = x%p;
  long y = 0;

  for(int i = 0; i<p; i++){
  	if(perfect(p*i+mod)){
		y = sqrt(p*i+mod);
		break;
	}
  }

  printf("sqrt(%ld) = %ld (mod %ld)\n",x,y,p);
  return 0;
}
int perfect(long number){
	long double sr = sqrt(number);
	return ((sr-floor(sr)) == 0);
}
