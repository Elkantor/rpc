/*
** listener.c -- a datagram sockets "server" demo
*/

#include "dependencies.h"

#define config rpc_server_config
#define helper rpc_helper_network

#define PORT "4950"    // the port users will be connecting to


int main(void)
{
    struct sockaddr_storage their_addr;
    char buffer[config.buffer_length_max];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    struct addrinfo hints   = { 0 };
    hints.ai_family         = AF_INET6;     // set to AF_INET to use IPv4
    hints.ai_socktype       = SOCK_DGRAM;   // UDP datagrams
    hints.ai_flags          = AI_PASSIVE;   // use my IP

    struct addrinfo* servinfo;
    {
        const int rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
        if (rv != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
        }
    }

    // loop through all the results and bind to the first we can
    int sockfd = 0;
    struct addrinfo* p;
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    int numbytes;
    if ((numbytes = recvfrom(sockfd, buffer, config.buffer_length_max -1 , 0,
        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
            helper.sockaddr_get((struct sockaddr *)&their_addr),
            s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buffer[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buffer);

    close(sockfd);

    return 0;
}