#include "Acceptor.h"

#include "SocketOpts.h"
#include "Address.h"

#include "EventLoop.h"

using namespace sockets;

Acceptor::Acceptor(EventLoop* loop, const Address& listenAddr)
    : loop_(loop),
      acceptSocket_(sockets::creatNonblocking(AF_INET)),
      acceptChannel_(loop, acceptSocket_.getSockfd()),
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
  Address perrAddr(0);
  int confd = acceptSocket_.accept(&perrAddr);
  if (confd >= 0) {
    if (newConnectionBack_)
      newConnectionBack_(confd, perrAddr);
    else
      sockets::close(confd);
  } else {
    if (errno == EMFILE) ::close(confd);
  }
}
