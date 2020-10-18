#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "myshell.h"
#include <errno.h>


extern char *filename;
extern int stdoutcopy;
extern bool file;
extern char *command;
extern char **options;

void parseInput(char* input)
{
    input = strtok(input, "\n");
            if(strchr(input,'>') != NULL)
            {
                filename = NULL;
                char *temp1 = strtok(input,">");
                filename = strtok(NULL," ");
                int f = open(filename, O_WRONLY|O_CREAT|O_TRUNC,0666);
                stdoutcopy = dup(1);
                dup2(f,1);
                file = true;
                input = temp1;

            }

            command = NULL;
            char *temp = NULL;
            options = malloc(sizeof(char*));
            temp = strtok(input, " ");
            command = temp;
            int i = 0;
            
            while(temp != NULL)
            {
                options[i] = temp;
                i++;
                options = realloc(options, (i+1) * sizeof(char *));
                temp = strtok(NULL, " ");
            }
            options[i] = NULL;
}