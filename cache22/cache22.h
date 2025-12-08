#ifndef CACHE22
#define CACHE22
#define _GNU_SOURCE



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include<assert.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdarg.h>


#define PORT "12049"
#define HOST "127.0.0.1"
#define log(x, args ...) \
    printf(x,##args)

typedef unsigned int int32;
typedef unsigned int int16;
typedef unsigned char int8;

struct s_client {
    int s; //socket file descriptor for the client
    char ip[16]; //ip address of the client
    int16 port;
};

typedef struct s_client Client;

void mainloop(int);
int initServer(int16);
int main(int,char**);
void zero(int8 *,int16);
void childloop(Client *);

#endif