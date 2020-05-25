#include <netdb.h>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>//结构体addrinfo, in_addr
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    /*struct hostent* phost = gethostent();
    printf("host name: %s\n", phost->h_name);
    printf("addr type: %d \n", phost->h_addrtype);
    printf("addr aliases: %s \n", phost->h_aliases);

    struct netent * pnet = getnetent();
    printf("net name: %s \n", pnet->n_name);*/


    char *hostname;
    struct addrinfo hints,*result, *rp;
    int err;
    in_addr addr;

    //没有输入域名时，博客园的网址，返回实际IP地址
    hostname = argc < 2 ? (char *)"www.cnblogs.com" : argv[1];

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;


    if((err = getaddrinfo(hostname, NULL, &hints, &result)) != 0){
        printf("error %d : %s\n", err, gai_strerror(err));
        return 1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        addr.s_addr = ((sockaddr_in*)(rp->ai_addr))->sin_addr.s_addr;
        printf("ip addresss: %s\n", inet_ntoa(addr));
    }

    freeaddrinfo(result);

    return 0;
}