#pragma once
#include <stdint.h>
#include <arpa/inet.h>
#include <stdbool.h>

typedef struct AddrV4 AddrV4;

struct AddrV4{
    char host[128];
    uint16_t port;
    struct sockaddr_in addr;
};

bool sock_make_nonlock(int fd);
bool addr_v4_init(AddrV4 *addr, const char *host, uint16_t port);
