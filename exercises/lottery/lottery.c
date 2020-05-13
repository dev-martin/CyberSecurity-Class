/* lottery.c */

#include <stdio.h>    /* for printf() */
#include <stdlib.h>   /* for rand() and srand() */
#include <sys/time.h> /* for gettimeofday() */

int your_fcn()
{
    /* Modify this function to win the "lottery" in main(). */

    return 0;
}

int main()
{
    /* Seed the random number generator */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    int rv;
    rv = your_fcn();

    /* Lottery time */
    if(rv != rand())
        printf("You lose\n");
    else
        printf("You win!\n");

    return EXIT_SUCCESS;
}
