#pragma once
#include <stdlib.h>
#include <sys/epoll.h>

#define DEFAULT_NUM_CONNECTIONS 32
typedef struct Conn Conn;
typedef struct Server Server;

struct Conn{
    int fd;
};

struct Server{
    int fd;
    Conn ** connections;
    size_t num_connections;
    size_t cap_connections;
    int epoll_fd;
    struct epoll_event events[DEFAULT_NUM_CONNECTIONS];
};

bool server_accept_conn(Server *server);
bool server_init(Server *server, const char * host, uint16_t port);
