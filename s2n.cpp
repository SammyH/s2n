#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define S2MAGIC 0xf197de9a

enum
{
    S2Cmd_ClientSnapshot=0xC7
};

typedef ssize_t(*tsendto)(int,const void*,size_t,int,const struct sockaddr*,socklen_t);
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
    static tsendto osendto = NULL;
    if(osendto == NULL)
    {
        osendto = (tsendto)dlsym(RTLD_NEXT, "sendto"); 
    }

    //printf("sendto(%d, %p, %zu, %d, %p, %lu)\r\n",
    //    sockfd, buf, len, flags, dest_addr, (unsigned long)addrlen);

    return osendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
    ssize_t r = syscall(SYS_recvfrom, sockfd, buf, len, flags, src_addr, addrlen);

    if(r > 8)
    {
	unsigned char* pkt = (unsigned char*)buf;
        if(*(unsigned int*)pkt == S2MAGIC)
        {
            unsigned char cmd = pkt[7];
            unsigned char* data = &pkt[8];
            if(cmd == S2Cmd_ClientSnapshot)
            {
                printf("Received client snapshot, frame %d\r\n",
                    *(unsigned short*)&data[4]);
            }
        }
    }

    return r;
}

