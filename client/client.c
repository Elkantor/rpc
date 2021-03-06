/*
** talker.c -- a datagram "client" demo
*/

#include "dependencies.h"

#define SERVERPORT "4950"    // the port users will be connecting to

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }

    struct addrinfo hints = {0};
    hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;

    // fill server info
    struct addrinfo* servinfo;
    {
        const int rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo);
        if (rv != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }
    }

    int sockfd;
    struct addrinfo* p;
    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) 
        {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    const int numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen);
    if (numbytes == -1) 
    {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfd);

    return 0;
}