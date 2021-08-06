# rpc
RPC client/server in C using UDP

## Build the program

### On Linux
With gcc:

*server*
```bash
cd server && gcc server.c -o server
./server
```

*client*
```bash
cd client && gcc client.c -o client
./client IPv6 message
```
