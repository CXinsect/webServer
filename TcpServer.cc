#include "TcpServer.h"
#include "SocketOpts.h"
#include "Address.h"
#include "TcpConnection.h"
#include "Acceptor.h"
using namespace placeholders;

void defaultConnectionCallback(const TcpConnectionPtr& conn) {
    std::cout << "Connected" << std::endl;
}
extern void defaultMessageCallback(const TcpConnectionPtr&conn,
                                    Buffer * buf)
{
    buf->retrieveAllBytes();
}
TcpServer::TcpServer(EventLoop* loop,
                          const Address &listenAddr,
                          const std::string& name)
    : loop_(loop),
    name_(name),
    acceptor_(new Acceptor(loop,listenAddr)),
    connectionBack_(defaultConnectionCallback),
    messageBack_(defaultMessageCallback),
    nextConfd_(1)
{
    acceptor_->setNewConnectionCallBack(
        std::bind(&TcpServer::newConnection,this,_1,_2));
}
void TcpServer::newConnection (int sockfd,const Address& peerAddr) {
    char buf[32];
    snprintf(buf,sizeof(buf),"#%d",nextConfd_);
    ++nextConfd_;
    std::string name = name_ + buf;
    std::cout << "TcpServer::New Connection: " << name_ 
              << "from " << peerAddr.toIpPort().c_str() << std::endl;
    // struct sockaddr_in addr_= sockets::getLocalAddr(sockfd);
    Address localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn (new TcpConnection(loop_,name,sockfd,localAddr,peerAddr));
    connection_[name] = conn;
    conn->setConnectionCallBack(connectionBack_);
    conn->setMessageCallBack(messageBack_);
    conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, _1));
    conn->connectEstablished();
}
void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    std::cout << "removeConnection: "<< name_ <<std::endl;

    size_t n = connection_.erase(conn->getName());
    assert(n == 1);
    conn->handClose();
}
void TcpServer::start() {
    acceptor_->Listen();
}
TcpServer::~TcpServer() {
    std::cout << "TcpServer close: " << name_ << std::endl;
    for(auto&item : connection_) {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->handClose();
    }
}