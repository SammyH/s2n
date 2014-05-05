#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

#include "Prerequisites.h"
#include "S2Daemon.h"

typedef ssize_t(*tsendto)(int,const void*,size_t,int,const struct sockaddr*,socklen_t);
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
    static tsendto osendto = NULL;
    if(osendto == NULL)
    {
        osendto = (tsendto)dlsym(RTLD_NEXT, "sendto"); 
    }

    return osendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
    ssize_t r = syscall(SYS_recvfrom, sockfd, buf, len, flags, src_addr, addrlen);

    if(g_pDaemon == NULL)
    {
        g_pDaemon = new S2Daemon();
        printf("Initialized S2Daemon.\r\n");
    }
    if(g_pDaemon != NULL)
    {
        r = g_pDaemon->OnReceivePacket((unsigned char*)buf, r);
    }

    return r;
}
