#include "cache22.h"

bool scontinuation;
bool ccontinuation;

void zero(int8* buf, int16 size){
    int8 *p;
    int16 n;
    for(n=0,p=buf;n<size;n++,p++)
        *p=0;

    return;
}

void childloop(Client *client){
    sleep(1);
    return ;
}

void mainloop(int s){
    struct sockaddr_in cli;
    int32 len;
    int s2;
    char *ip;
    int16 port;
    Client *client;
    pid_t pid;

    s2 = accept(s, (struct sockaddr *)&cli,(unsigned int *)&len);
    if(s2<0){
        return;
    }

    port = ntohs(cli.sin_port);
    ip = inet_ntoa(cli.sin_addr);

    log("Client connected from %s:%d\n", ip, port);
    fflush(stdout);

    client = (Client *)malloc(sizeof(Client));
    zero((int8 *)client, sizeof(struct s_client));
    assert(client);
    client->s = s2;
    client->port = port;
    strncpy(client->ip,ip,15);

    pid = fork();
    if(pid){
        free(client);
        return;
    }
    else{
        dprintf(s2, "100 Connected to Cache22 server");
        ccontinuation = true;
        while(ccontinuation){
            childloop(client);
        }
        close(s2);
        free(client);
        return ;
    }

}

int initServer(int16 port){
    struct sockaddr_in sock;
    int s;
    int opt = 1; 

    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    sock.sin_addr.s_addr = inet_addr(HOST);

    s = socket(AF_INET, SOCK_STREAM,0);
    assert(s>0);

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    errno = 0;
    if(bind(s, (struct sockaddr *)&sock, sizeof(sock)))
        assert_perror(errno);

    errno = 0;
    if(listen(s, 10))
        assert_perror(errno);

    log("Server listening on port %d, %s\n", port, HOST);
    fflush(stdout); 

    return s;
}

int main(int argc, char *argv[]){
    char *sport;
    int s;
    int16 port;
    if(argc < 2)
        sport =(char *) PORT;
    else{
        sport = argv[1];
    }
    port = (int16)atoi(sport);
    scontinuation = true;
    s = initServer(port);
    while(scontinuation){       // ← ADD LOOP!
        mainloop(s);
    }

    close(s);
    
    return 1;
}