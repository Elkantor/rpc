/*
** rpc_helper_network -- contains helper methods to do network
*/

#if defined(rpc_helper_network_self_contain)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <stddef.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

// get sockaddr IPv4 or IPv6 address:
void* sockaddr_get(const struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// create a udp socket with ipv6 on port passed in parameter, return the socket file descriptor
int udp_ipv6_socket_create(const char* port)
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


const struct rpc_helper_network
{
    void*   (*sockaddr_get)(const struct sockaddr* sa);
    int     (*udp_ipv6_socket_create)(const char* port);
}
rpc_helper_network =
{
    .sockaddr_get = sockaddr_get,
    .udp_ipv6_socket_create = udp_ipv6_socket_create,
};