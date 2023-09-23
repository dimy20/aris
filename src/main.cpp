#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>

#define LOG_ERR(s) fprintf(stderr, "Error: %s\n", s)
#define TODO(s) fprintf(stderr, "TODO at %s:%d => %s\n", __FILE__, __LINE__, s); exit(1);

#define HOST "127.0.0.1"
#define PORT 8080
#define DEFAULT_NUM_CONNECTIONS 32

#define MALLOC(size) ({ \
    void *ptr = malloc(size); \
    if (!ptr) { \
        fprintf(stderr, "Failed to allocate memory: %s:%d\n", __FILE__, __LINE__);\
        exit(EXIT_FAILURE); \
    } \
    ptr; \
})

struct AddrV4{
    char host[128];
    uint16_t port;
    struct sockaddr_in addr;
};

static bool sock_make_nonlock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);

    if(flags == -1){
        return false;
    }

    flags |= O_NONBLOCK;

    return fcntl(fd, F_SETFL, flags) == 0;
}

static bool addr_v4_init(AddrV4 *addr, const char *host, uint16_t port){
    if(inet_pton(AF_INET, host, &addr->addr.sin_addr) < 0){
        return false;
    }
    addr->addr.sin_family = AF_INET;
    addr->addr.sin_port = htons(port);
    addr->port = port;
    memcpy(addr->host, host, 128);
    return true;
};

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

bool server_init_epoll(Server *server){
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

static bool server_accept_conn(Server *server){
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

Server server;

int main(){
    if(!server_init(&server, HOST, PORT)){
        exit(1);
    };

    while(true){
        int num_events;
        num_events = epoll_wait(server.epoll_fd, server.events, DEFAULT_NUM_CONNECTIONS, -1);
        if(num_events < 0){
            LOG_ERR(strerror(errno));
            exit(1);
        }else{
            for(int i = 0; i < num_events; i++){
                if(server.events[i].events & EPOLLIN){
                    if(server.events[i].data.fd == server.fd){
                        if(!server_accept_conn(&server)){
                            exit(1);
                        }
                        printf("New connection!\n");
                    }else{
                        TODO("Handle conn read events");
                        //handle connections
                    }
                };

                if(server.events[i].events & EPOLLOUT){
                    TODO("Handle write events");
                }
            }
        }
    };
    std::cout  << "Hello, World";
};
