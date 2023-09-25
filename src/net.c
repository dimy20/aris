#include "net.h"
#include <fcntl.h>
#include <string.h>

bool sock_make_nonlock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);

    if(flags == -1){
        return false;
    }

    flags |= O_NONBLOCK;

    return fcntl(fd, F_SETFL, flags) == 0;
}

bool addr_v4_init(AddrV4 *addr, const char *host, uint16_t port){
    if(inet_pton(AF_INET, host, &addr->addr.sin_addr) < 0){
        return false;
    }
    addr->addr.sin_family = AF_INET;
    addr->addr.sin_port = htons(port);
    addr->port = port;
    memcpy(addr->host, host, 128);
    return true;
};
