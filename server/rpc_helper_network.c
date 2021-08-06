/*
** rpc_helper_network -- contains helper methods to do network
** Dependencies :
**  - <sys/socket.h>
*/

// get sockaddr IPv4 or IPv6 address:
void* sockaddr_get(const struct sockaddr* _sa)
{
    if (_sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)_sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)_sa)->sin6_addr);
}


const struct rpc_helper_network
{
    void* (*sockaddr_get)(const struct sockaddr* _sa);
}
rpc_helper_network =
{
    .sockaddr_get = sockaddr_get,
};