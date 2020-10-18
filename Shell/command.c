#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "myshell.h"
#include <errno.h>


void executeCommand(char* command, char** options)
{
    if(strcmp(command,"cd") == 0)
    {
        if(options[1] == NULL)
        {
            if(getenv("HOME") == NULL)
            {
                perror("Home path does not exist\n");
            }else
            {
                chdir(getenv("HOME"));
            }    
        }
        else if(chdir(options[1]) == -1)
        {
            printf("cd: %s: ", options[1]);
            fflush(stdout);
            perror("");
        }
    }else
    {
        pid_t childPid;
        int childStatus;
        childPid = fork();
        
        if(childPid == 0)
        {
            execvp(command, options);
            printf("Unknown command\n");
            exit(0);
        }else if(childPid < 0)
        {
            printf("Error creating fork");
        }else
        {
            wait(&childStatus);
        }
    }
}