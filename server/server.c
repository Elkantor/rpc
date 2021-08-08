/*
** listener.c -- a datagram sockets "server" demo
*/

#include "dependencies.h"

int main(void)
{
    const int socket_file_descriptor = rpc_helper_network_udp_ipv6_socket_create(rpc_server_config_port);
    printf("listener: waiting to recvfrom...\n");

    while(1)
    {
        struct sockaddr_storage their_addr;
        socklen_t addr_len = sizeof(their_addr);
        char buffer[rpc_server_config_buffer_length_max];

        // receive data from socket
        const int numbytes = recvfrom(socket_file_descriptor, buffer, rpc_server_config_buffer_length_max -1 , 0, (struct sockaddr*)&their_addr, &addr_len);
        if (numbytes  == -1)
        {
            perror("recvfrom");
            exit(1);
        }

        char s[INET6_ADDRSTRLEN];
        void* client_ip = rpc_helper_network_sockaddr_get((struct sockaddr *)&their_addr);
        const char* client_addr = inet_ntop(their_addr.ss_family, client_ip, s, sizeof(s));
        
        printf("listener: got packet from %s\n", client_addr);
        printf("listener: packet is %d bytes long\n", numbytes);
        buffer[numbytes] = '\0';
        printf("listener: packet contains \"%s\"\n", buffer);
    }

    close(socket_file_descriptor);

    return 0;
}