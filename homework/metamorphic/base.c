#include <string.h>
#include <stdio.h>

int main()
{
  int a = 5;
  int b = 5;
  int c = a + b;
  int keep = 30;
  while( keep > 1)
  {
	  for(int i = 0; i < 30; i++){
		  a = a*i;
		  b = (c - a )* i;
		  if(b<a)
		  {
			  for(int j=i ; j< 10; j++){
				  c = c-b;
				  if(a+b>c)
					  printf("%d aaaaaaaaaaaaaaaaaaaa\n",a);
				  else
					  printf("%d cccccccccccccccccccc\n",c);
		  
			  }

		  }
  	    }
	    if(keep<30)
		    keep = keep-4;
	    else
		    keep--;
	    printf("%d aaaaaaaaaaaaaaaaaaaa\n",a);
  }
  return 0;
}
