/*************************************************************
 * This is the client which serves as a worker process. Each
 * client calculates a single slice of the the resulting matrix
 * in the master process. The worker process is complete after
 * it sends back the calculated slice to the master.
 * **********************************************************/

#define _DEFAULT_SOURCE /* For NI_MAXHOST and NI_MAXSERV */
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define PORTNUM "49937"    /* Port number for server */
#define BACKLOG 10
#define BUFSIZE 32

int main(int argc, char *argv[]) 
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL; 
    hints.ai_addr = NULL;
    hints.ai_next = NULL; 
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(argv[1], PORTNUM, &hints, &result) != 0)
       exit(-1);

    int cfd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
         cfd = socket(rp->ai_family, rp->ai_socktype, 
                      rp->ai_protocol);

         if (cfd == -1)
            continue;   /* On error, try next address */

         if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */

         /* close() failed, close this socket, try next address */
         close(cfd);
    }

    if (rp == NULL)
       exit(-1);

    freeaddrinfo(result);

    {
        int n;
        int p;
        recv(cfd,&n,sizeof n,0);
        recv(cfd,&p,sizeof p,0);
        float B[n][n];
        recv(cfd,&B,sizeof(B),0);
        float ASLICE[n/p][n];
        recv(cfd,&ASLICE, sizeof(ASLICE),0);
        float CSLICE[n/p][n];

         for(int i = 0;i<(n/p);i++)
         {
            for(int j = 0;j<n;j++)
            {
               CSLICE[i][j] = 0;
               for(int z = 0; z < n;z++)
               {
                  CSLICE[i][j]+= ASLICE[i][z] * B[z][j];
               }
            }
         }

         send(cfd,&CSLICE,sizeof(CSLICE),0);

    }

    if (close(cfd) == -1) /* Close connection */
    {
        fprintf(stderr, "close error.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

