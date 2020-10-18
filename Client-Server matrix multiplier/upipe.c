
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define BUFSIZE 10
int main(int argc, char *argv[])
{
    int pfd[2];
    char buf[BUFSIZE];
    
    if (pipe(pfd) == -1)
       exit(EXIT_FAILURE);

    switch (fork()) {
    case -1:
       	exit(EXIT_FAILURE);

    case 0:
        if (close(pfd[1]) == -1)
           exit(EXIT_FAILURE);

        fprintf(stdout, "Child received \n");
        fflush(stdout);

        for (;;) {
           ssize_t numRead = read(pfd[0], buf, BUFSIZE);
           if (numRead == -1)
              exit(EXIT_FAILURE);
           if (numRead == 0)
              break;
           if (write(1, buf, numRead) != numRead)
              exit(EXIT_FAILURE);
        }
        write(1, "\n", 1);
        if (close(pfd[0]) == -1)
           exit(EXIT_FAILURE);
        _exit(EXIT_SUCCESS);

    default:
        if (close(pfd[0]) == -1)
           exit(EXIT_FAILURE);

        fprintf(stdout, "Parent: Writing \'%s\' to child.\n", argv[1]);
        fflush(stdout);

        if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
           exit(EXIT_FAILURE);
        if (close(pfd[1]) == -1)
           exit(EXIT_FAILURE);
        wait(NULL);
    }

    exit(EXIT_SUCCESS);
}

