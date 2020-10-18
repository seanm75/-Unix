/***********************************************************
 * This is the server which serves as the master process
 * Each client is a worker process.The master process
 * is complete when all of the worker processes have joined
 * and sent back the slice of c they calculated. At which
 * point C is printed.
 * ********************************************************/

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
#include <pthread.h>

#define PORTNUM "49937"    /* Port number for server */
#define BACKLOG 10
#define BUFSIZE 32

int n;
int p;
int worker = 0;
float *slicep;
float *bp;
float *cp;
pthread_mutex_t mutex;

void printMatrix(int n, float M[n][n]);

void*
handleRequest(void* input)
{
    int cfd = *(int*)input;
    
    pthread_mutex_lock(&mutex);
    send(cfd,&n,sizeof(n),0);
    send(cfd,&p, sizeof(p),0);
    send(cfd,bp,sizeof(bp)*(n*n)/2,0);
    send(cfd,slicep,sizeof(slicep)*(n*(n/p))/2,0);
    float cslice[n/p][n];
    recv(cfd,&cslice,sizeof(cslice),0);
    for(int i = 0;i<(n/p);i++)
    {
       for(int j = 0;j<n;j++)
       {
          *(cp+j+((i+(worker*(n/p)))*n)) = cslice[i][j];
       }
    }

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) 
{
   printf("Please enter the size of the matrix\n");
   scanf("%d", &n);
   printf("Please enter the number of worker processes\n");
   scanf("%d", &p);
   float A[n][n];
   float B[n][n];
   bp = &B[0][0];
   float C[n][n];
   cp = &C[0][0];
   float ASLICE[n/p][n];
   slicep = &ASLICE[0][0];

   for(int i = 0;i<n;i++)
   {
      for(int j = 0;j<n;j++)
      {
         A[i][j] = 2;
         B[i][j] = 2;
      }
   }

   printf("\nA:");
   printMatrix(n,A);
   printf("\nB:");
   printMatrix(n,B);



    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL; 
    hints.ai_addr = NULL;
    hints.ai_next = NULL; 
    hints.ai_family = AF_INET; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if (getaddrinfo(NULL, PORTNUM, &hints, &result) != 0)
       exit(-1);

    int lfd, optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
         lfd = socket(rp->ai_family, rp->ai_socktype, 
                      rp->ai_protocol);

         if (lfd == -1)
            continue;   /* On error, try next address */

         if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, 
                        &optval, sizeof(optval)) == -1)
            exit(-1);

         if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Success */

         /* bind() failed, close this socket, try next address */
         close(lfd);
    }

    if (rp == NULL)
       exit(-1);

    {
       char host[NI_MAXHOST];
       char service[NI_MAXSERV];
       if (getnameinfo((struct sockaddr *)rp->ai_addr, rp->ai_addrlen,
                 host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
          fprintf(stdout, "Listening on (%s, %s)\nFor connecting worker processes...\n", host, service);
       else
          exit(-1);
    }

    freeaddrinfo(result);

    if (listen(lfd, BACKLOG) == -1)
       exit(-1);

    pthread_mutex_init(&mutex,NULL);
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_JOINABLE);

    while(worker < p)
    {
        int cfd = accept(lfd, NULL, NULL);
        if (cfd == -1) {
           continue;   /* Print an error message */
        }
        printf("worker process (%d/%d) connected\n", worker+1, p);
        for(int i = 0;i<(n/p);i++)
        {
           for(int j = 0;j<n;j++)
           {
              ASLICE[i][j] = A[(worker*(n/p))+i][j];
           }
        }

        pthread_t t;
        pthread_create(&t, &tattr, handleRequest, &cfd);
        pthread_join(t,NULL);
        worker++;
        if (close(cfd) == -1) /* Close connection */
        {
           fprintf(stderr, "close error.\n");
           exit(EXIT_FAILURE);
        }
    }
        
    pthread_mutex_destroy(&mutex);
    pthread_attr_destroy(&tattr); 

    printf("\nC:");
    printMatrix(n,C);
    if (close(lfd) == -1) /* Close connection */
        {
           fprintf(stderr, "close error.\n");
           exit(EXIT_FAILURE);
        }
    exit(EXIT_SUCCESS);
}


void printMatrix(int n, float M[n][n])
{
   for(int i = 0;i<n;i++)
   {
      printf("\n  ");
      for(int j = 0;j<n;j++)
      {
         printf("%.2f ", M[i][j]);
      }
   }
   printf("\n\n");
}