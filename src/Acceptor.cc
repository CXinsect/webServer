#include "Acceptor.h"

#include "Address.h"
#include "EventLoop.h"
#include "SocketOpts.h"

using namespace sockets;

Acceptor::Acceptor(EventLoop* loop, const Address& listenAddr)
    : loop_(loop),
      acceptSocket_(sockets::creatNonblocking(AF_INET)),
      acceptChannel_(loop, acceptSocket_.getSockfd()),
      clientCount_(0),
      listening_(false) {
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.setReadCallBack(bind(&Acceptor::handleRead, this));
}
void Acceptor::Listen() {
  listening_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
  loop_->assertNotInLoopThread();
  Address perrAddr(0);
  int confd = acceptSocket_.accept(&perrAddr);
  if (newConnectionBack_)
    newConnectionBack_(confd, perrAddr);
  else {
    ::close(confd);
  }
}
