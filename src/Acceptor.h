#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include <atomic>
#include <functional>

#include "Channel.h"
#include "Socket.h"
class EventLoop;
class Address;
class Socket;
class Channel;

using namespace std;

class Acceptor {
 public:
  typedef std::function<void(int sockfd, const Address &)>
      NewConnectionCallBack;
  Acceptor(EventLoop *loop, const Address &lisAddr);
  void setNewConnectionCallBack(const NewConnectionCallBack &cb) {
    newConnectionBack_ = cb;
  }
  bool listenning() const { return listening_; }
  void Listen();

 private:
  void handleRead();
  EventLoop *loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallBack newConnectionBack_;
  bool listening_;
  int conn_;
  std::atomic<int> clientCount_;
};
#endif