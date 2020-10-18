#include <signal.h>
#include <stdio.h>
#include "myshell.h"

void signalHandling(int signo)
{
    if(signo == SIGINT)
    {
        printf("\n");
        getTime();
        printf("# ");
        fflush(stdout);
    }
}