#include "Socket.h"
#include "Address.h"
#include "SocketOpts.h"
Socket::~Socket() { ::close(sockfd); }

void Socket::bindAddress(const Address &addr) {
    sockets::Bind(sockfd,addr.getSockAddr());
}
void Socket::listen() {
    sockets::Listen(sockfd);
}
void Socket::accept(Address *peeraddr,vector<int>& v) {
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    while(true) {
        int confd = sockets::accept(sockfd,&addr);
        if(confd < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK) break;
            else ::close(confd);
        }
        v.push_back(confd);
    }
    return;
}

void Socket::Close() {
    ::close(sockfd);
}

void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    int  ret = ::setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&optval
                                    ,static_cast<socklen_t>(sizeof(optval)));
    if(ret < 0 && on) 
        sockets::myError("setsockopt",__LINE__);
}
void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,
                        static_cast<socklen_t>(sizeof(optval)));
    if(ret < 0 && on) 
        sockets::myError("setsockopt",__LINE__);
}
