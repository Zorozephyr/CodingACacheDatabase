#include "cache22.h"

int32 handle_hello(Client *, int8*, int8*);
bool scontinuation;
bool ccontinuation;


CmdHandler handlers[] = {
    {(int8 *)"hello", handle_hello}
};

int32 handle_hello(Client *cli,int8* folder, int8 *args){
    dprintf(cli->s,"hello '%s'\n",folder);
    return 0;
}

Callback getcmd(int8 *cmd){
    Callback cb;
    int16 n,arrlen;

    arrlen = sizeof(handlers)/sizeof(handlers[0]);
    cb=0;
    for(n=0;n<arrlen;n++){
        if(!strcmp((char *)cmd,(char*)handlers[n].cmd)){
            cb = handlers[n].handler;
        }
    }

    return cb;

}

void zero(int8* buf, int16 size){
    int8 *p;
    int16 n;
    for(n=0,p=buf;n<size;n++,p++)
        *p=0;

    return;
}

void childloop(Client *cli){
    int8 buf[256];
    int16 n;
    int8 *p,*f;
    int8 cmd[256], folder[256], args[256];
    zero(cmd,256);
    zero(folder,256);
    zero(args,256);

    zero(buf,256);
    ssize_t bytes_read = read(cli->s, (char *) buf, 255);
    if (bytes_read <= 0) {
        return; 
    }
    n = (int16)strlen((char *)buf);

    if(n>254){
        n=254;
    }

    for(p=buf;(*p) && (n--) && (*p != ' ') && (*p!='\n')&& (*p!='\r');p++);
    if(!(*p)){
        strncpy((char *)cmd, (char *)buf,255);
    }
    else if((*p == '\n') || (*p=='\r')){
        *p=0;
        strncpy((char *)cmd, (char *)buf,255);
        goto done;
    }
    else if((*p==' ')){
        *p=0;
        strncpy((char *)cmd, (char *)buf,255);
    }

    for(p++,f=p;(*p) && (n--) && (*p != ' ') && (*p!='\n')&& (*p!='\r');p++){

    }
    if(!(*p) || (!n)){
        strncpy((char *)folder, (char *)f,255);
        goto done;
    }
    else if((*p==' ') || (*p == '\n') || (*p=='\r')){
        *p=0;
        strncpy((char *)folder, (char *)f,255);
    }

    p++;
    if(*p){
        strncpy((char *)args, (char *)p,255);
        for(p=args;(*p) && (*p!='\n') && (*p!='\r');p++);
        *p=0;
    }

    done:
        dprintf(cli->s, "cmd:\t%s\n",cmd);
        dprintf(cli->s, "folder:\t%s\n",folder);
        dprintf(cli->s, "args:\t%s\n",args);

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

    //tempCode
    Callback x;
    x=getcmd((int8*)"hello");
    printf("%p\n",x);
    return 0;
    //tempCode end
    port = (int16)atoi(sport);
    scontinuation = true;
    s = initServer(port);
    while(scontinuation){       // ← ADD LOOP!
        mainloop(s);
    }

    close(s);
    
    return 1;
}

#pragma GCC diagnostic pop