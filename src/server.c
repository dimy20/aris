#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "server.h"
#include "net.h"
#include "err.h"

static bool server_init_epoll(Server *server){
    server->epoll_fd = epoll_create1(0);

    if(server->epoll_fd < 0){
        return false;
    }

    struct epoll_event ev;
    ev.data.fd = server->fd;
    ev.events |= EPOLLIN;

    memset(server->events, 0, sizeof(server->events));
    return epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->fd, &ev) == 0;
};

bool server_accept_conn(Server *server){
    int conn_fd = accept(server->fd, NULL, NULL);
    if(conn_fd < 0){
        LOG_ERR(strerror(errno));
        return false;
    }

    //Initialize the socket
    if(!sock_make_nonlock(conn_fd)) return false;
    // add socket to epoll

    struct epoll_event ev;
    ev.data.fd = conn_fd;
    ev.events = EPOLLIN;

    if(epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0){
        LOG_ERR(strerror(errno));
        return false;
    }

    // add socket to connections
    Conn *new_conn = (Conn *)MALLOC(sizeof(Conn*));
    new_conn->fd = conn_fd;


    // Guard insert
    if(server->num_connections == server->cap_connections){
        size_t new_cap = server->num_connections * 2;
        server->connections = (Conn **)realloc(server->connections, new_cap * sizeof(Conn *));
        server->num_connections = new_cap;
    };

    server->connections[conn_fd] = new_conn;
    server->num_connections += 1;

    return true;
};

bool server_init(Server *server, const char * host, uint16_t port){
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server->fd < 0){
        LOG_ERR(strerror(errno));
        return false;
    }

    int value = 1;
    if(setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0){
        return false;
    }

    if(!sock_make_nonlock(server->fd)){
        return false;
    }

    AddrV4 addr;
    if(!addr_v4_init(&addr, host, port)){
        return false;
    }

    if(bind(server->fd, (struct sockaddr *)&addr.addr, sizeof(struct sockaddr_in)) < 0){
        LOG_ERR(strerror(errno));
        return false;
    }

    server->connections = (Conn**)MALLOC(sizeof(Conn *) * DEFAULT_NUM_CONNECTIONS);
    server->cap_connections = DEFAULT_NUM_CONNECTIONS;
    server->num_connections = 0;
    for(int i = 0; i < server->cap_connections; i++){
        server->connections[i] = NULL;
    }

    if(!server_init_epoll(server)) return false;

    return listen(server->fd, 32) == 0;
};
