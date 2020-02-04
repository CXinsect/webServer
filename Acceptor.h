#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_
#include <functional>
#include "Socket.h"
#include "Address.h"
#include "EventLoop.h"
class Acceptor {
    public:
        typedef std::function <void (int sockfd,const Address &)> NewConnectionCallBack;
        Acceptor(EventLoop *loop,const Address &lisAddr);
        void setNewConnectionCallBack(const NewConnectionCallBack & cb) {
            newConnectionBack_ = cb;
        }
        bool listenning() const { return listening_; }
        void Listen();
        // const int getConn() { return conn_; }
    private:
        void handleRead();
        EventLoop *loop_;
        Socket acceptSocket_;
        Channel acceptChannel_;
        NewConnectionCallBack newConnectionBack_;
        bool listening_;
        int conn_;
};
#endif