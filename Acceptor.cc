#include "Acceptor.h"

#include "SocketOpts.h"
#include "Address.h"

#include "EventLoop.h"

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
  Address perrAddr(0);
  vector<int> v;
  acceptSocket_.accept(&perrAddr,v);
  for(int i = 0;i < v.size();i++) {
      clientCount_++;
      if(clientCount_ == 1020)
        ::close(v[i]);
      if (newConnectionBack_)
        newConnectionBack_(v[i], perrAddr);
      else {
        sockets::close(v[i]);
        clientCount_--;
      }
    }
}
