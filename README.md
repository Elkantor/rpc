# rpc
> Simple RPC client/server in C using UDP socket (currently only for linux)

## Installing / Getting started

1. [Linux (gcc or clang)](#on-linux)
2. [Windows (cl, gcc or clang) *soon*](#on-windows)

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
