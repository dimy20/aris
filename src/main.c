#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "server.h"
#include "net.h"
#include "err.h"
#define HOST "127.0.0.1"
#define PORT 8080

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
};
