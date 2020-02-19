#include "TcpServer.h"

#include "Acceptor.h"
#include "Address.h"
#include "SocketOpts.h"
#include "TcpConnection.h"
#include "threadLoopPool.h"

using namespace placeholders;

void defaultConnectionCallback(const TcpConnectionPtr& conn) {
  std::cout << "Connected" << std::endl;
}
extern void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer* buf) {
  buf->retrieveAllBytes();
}
TcpServer::TcpServer(EventLoop* loop, const Address& listenAddr,
                     const std::string& name)
    : loop_(loop),
      name_(name),
      acceptor_(new Acceptor(loop, listenAddr)),
      connectionBack_(defaultConnectionCallback),
      messageBack_(defaultMessageCallback),
      nextConfd_(1),
      threadLoopPool_(new threadLoopPool(loop_)) {
  acceptor_->setNewConnectionCallBack(
      std::bind(&TcpServer::newConnection, this, _1, _2));
}
void TcpServer::newConnection(int sockfd, const Address& peerAddr) {
  char buf[32];
  loop_->assertNotInLoopThread();
  snprintf(buf, sizeof(buf), "#%d", nextConfd_);
  ++nextConfd_;
  std::string name = name_ + buf;
  std::cout << "TcpServer::New Connection: " << name_ << "from "
            << peerAddr.toIpPort().c_str() << std::endl;
  Address localAddr(sockets::getLocalAddr(sockfd));
  EventLoop* t = threadLoopPool_->getNextloop();
  TcpConnectionPtr conn(
      new TcpConnection(t, name, sockfd, localAddr, peerAddr));
  connection_[name] = conn;
  conn->setConnectionCallBack(connectionBack_);
  conn->setMessageCallBack(messageBack_);
  conn->setCloseCallBack(std::bind(&TcpServer::removeConnection, this, _1));
  t->runInLoop(bind(&TcpConnection::connectEstablished, conn));
}
void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
  std::cout << "removeConnection: " << name_ << std::endl;
  loop_->runInLoop(bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
  loop_->assertNotInLoopThread();
  size_t n = connection_.erase(conn->getName());
  assert(n == 1);
  EventLoop* t = threadLoopPool_->getNextloop();
  t->queueInLoop(bind(&TcpConnection::connectionClose, conn));
}
void TcpServer::setThreadNums(int nums) {
  threadLoopPool_->setThreadnums(nums);
}
void TcpServer::start() {
  threadLoopPool_->start(threadCallback_);
  assert(!acceptor_->listenning());
  loop_->runInLoop(bind(&Acceptor::Listen, acceptor_.get()));
}
TcpServer::~TcpServer() {
  std::cout << "TcpServer close: " << name_ << std::endl;
  for (auto& item : connection_) {
    TcpConnectionPtr conn(item.second);
    item.second.reset();
    conn->getLoop()->runInLoop(bind(&TcpConnection::connectionClose, conn));
    ;
  }
}