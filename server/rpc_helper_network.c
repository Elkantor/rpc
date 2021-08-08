/*
** rpc_helper_network -- contains helper methods to do network
*/

#if defined(RPC_HELPER_NETWORK_SELF_CONTAINED)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <stddef.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

// get sockaddr IPv4 or IPv6 address:
void* rpc_helper_network_sockaddr_get(const struct sockaddr* sa)
{
    // ipv4 address
    if (sa->sa_family == AF_INET) 
    {
        struct sockaddr_in* socket_address_ipv4 = (struct sockaddr_in*)sa;
        struct in_addr*     socker_address_ip   = &(socket_address_ipv4->sin_addr);
        return socker_address_ip;
    }

    // ipv6 address
    struct sockaddr_in6*    socket_address_ipv6 = (struct sockaddr_in6*)sa;
    struct in6_addr*         socket_address_ip   = &(socket_address_ipv6->sin6_addr);
    return socket_address_ip;
}

// create a udp socket with ipv6 on port passed in parameter, return the socket file descriptor
int rpc_helper_network_udp_ipv6_socket_create(const char* port)
{
    struct addrinfo hints   = { 0 };
    hints.ai_family         = AF_INET6;     // set to AF_INET to use IPv4
    hints.ai_socktype       = SOCK_DGRAM;   // UDP datagrams
    hints.ai_flags          = AI_PASSIVE;   // use my IP

    struct addrinfo* servinfo;
    {
        const int rv = getaddrinfo(NULL, port, &hints, &servinfo);
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

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
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

    return sockfd;
}