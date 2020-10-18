#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "myshell.h"
#include <errno.h>

char *command = NULL;
char** options;
char *filename = NULL;
int stdoutcopy;
bool file = false;

int main(void)
{
    char *input = NULL;
    char *filename = NULL;
    size_t length = 0;
    size_t read;

    if(signal(SIGINT, signalHandling) == SIG_ERR)
    {
         printf("Error occured trying to catch");
         return EXIT_FAILURE;
    }

    while(true) //keeps running until ctrl+d entered
    {
        file = false;
        getTime();
        printf("# ");
        read = getline(&input, &length,stdin);
        if(feof(stdin))
        {
            printf("\n");
            exit(0);
        }

        if(strlen(input) > 1)
        {
            parseInput(input);
            executeCommand(command,options);

            if(file)
            {
                close(1);
                dup2(stdoutcopy,1);
            } 
        }
    }
    return 0;
}