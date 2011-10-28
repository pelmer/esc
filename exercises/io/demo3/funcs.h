#include <setjmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>


void *Malloc(size_t size)
{
    void *ptr;

    if ((ptr = malloc(size)) == NULL)
    {
        perror("malloc");
        exit(1);
    }
    return ptr;
}



FILE *Fopen(char *file, char *mode)
{
    FILE *fp;

    if ((fp = fopen(file, mode)) == NULL)
    {
         perror("fopen");
         exit(1);
    }
    return fp;
}



void Sigaction(int sig, const struct sigaction *sa,
                              struct sigaction *old)
{
    if (sigaction(sig, sa, NULL) < 0)
    {
        perror("sigaction");
        exit(1);
    }
}



void Sigfillset(sigset_t *mask)
{
    if (sigfillset(mask) < 0)
    {
         perror("sigfillset");
         exit(1);
    }
}



void Sigemptyset(sigset_t *mask)
{
    if (sigemptyset(mask) < 0)
    {
         perror("sigemptyset");
         exit(1);
    }
}


void Sigaddset(sigset_t *mask, int signum)
{
    if (sigaddset(mask, signum) < 0)
    {
         perror("sigaddset");
         exit(1);
    }
}    




void Sigprocmask(int how, const sigset_t *mask, sigset_t *old)
{
    if (sigprocmask(how, mask, old) < 0)
    {
          perror("sigprocmask");
          exit(1);
    }
}



int Socket(int family, int type, int protocol)
{
    int sockfd;

    if ((sockfd = socket(family, type, protocol)) < 0)
    {
        perror("socket");
        exit(1);
    }
    return sockfd;
}


void Connect(int sockfd, const struct sockaddr *saddr, socklen_t addrlen)
{
    if (connect(sockfd, saddr, addrlen) < 0)
    {
        perror("connect");
        exit(1);
    }
}


void Bind(int sockfd, const struct sockaddr *saddr, socklen_t addrlen)
{
    if (bind(sockfd, saddr, addrlen) < 0)
    {
        perror("bind");
        exit(1);
    }
}


void Listen(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) < 0)
    {
        perror("listen");
        exit(1);
    }
}


int Open(const char *path, int oflag, int modes)
{
    int fd;

    if ((fd = open(path, oflag, modes)) < 0)
    {
        perror("open");
        exit(1);
    }
    return fd;
}       


pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    return pid;
}


void Close(int fd)
{
    if (close(fd) < 0)
    {
        perror("close");
        exit(1);
    }
}



FILE *Popen(char *file, char *mode)
{
    FILE *fp;

    if ((fp = popen(file,mode)) < 0)
    {
        perror("popen");
        exit(1);
    }
    return fp;
}



void *Realloc(void *ptr, size_t size)
{
    void *newptr;

    if ((newptr = realloc(ptr, size)) < 0)
    {
         perror("realloc");
         exit(1);
    }
    return newptr;
}

