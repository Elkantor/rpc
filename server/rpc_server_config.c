const struct rpc_server_config
{
    const unsigned int      buffer_length_max;
    const char*             port;
} 
rpc_server_config = 
{
    .buffer_length_max  = 100,
    .port               = "4950",
};