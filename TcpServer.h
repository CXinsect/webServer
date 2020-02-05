#ifndef  _TCP_SERVER_H_
#define  _TCP_SERVER_H_
#include "EventLoop.h"
#include "Address.h"
#include "modelHead.h"
class TcpConnection;
class Acceptor;

class TcpServer {
    public:
    TcpServer(EventLoop *loop,
             const Address &localAddr,
             const std::string& Name);
    //暂时不处理
    ~TcpServer();
    void start();
    void setConnectionCallBack(const ConnectionCallBack& cb) {
        connectionBack_ = cb;
    }
    void setMessageCallBack(const MessageCallBack cb) {
        messageBack_ = cb;
    }
    void SetCloseCallBack(const CloseCallBack& cb) {
        closeCallBack_ = cb;
    }
    // void removeConnection(const TcpConnectionPtr& conn);
    
    private:
        void removeConnection(const TcpConnectionPtr& conn);
        void removeConnectInLoop(const TcpConnectionPtr& conn); 
        void newConnection(int sockfd,const Address &peerAddr);
        typedef std::map<std::string,TcpConnectionPtr> ConnectionMap;
        EventLoop *loop_;
        const std::string name_;
        bool started_;
        int nextConfd_;
        boost::scoped_ptr <Acceptor> acceptor_;
        ConnectionCallBack connectionBack_;
        MessageCallBack messageBack_;
        CloseCallBack closeCallBack_;
        ConnectionMap connection_;
};

#endif