#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_
#include "Address.h"
#include "EventLoop.h"
#include "modelHead.h"

class TcpConnection;
class Acceptor;
class threadLoopPool;

class TcpServer {
 public:
  TcpServer(EventLoop* loop, const Address& localAddr, const std::string& Name);
  ~TcpServer();
  void start();
  void setConnectionCallBack(const ConnectionCallBack& cb) {
    connectionBack_ = cb;
  }
  void setMessageCallBack(const MessageCallBack cb) { messageBack_ = cb; }
  void SetCloseCallBack(const CloseCallBack& cb) { closeCallBack_ = cb; }
  void setThreadCallBack(const threadFuncType& cb) { threadCallback_ = cb; }
  void setThreadNums(int nums);

 private:
  void removeConnection(const TcpConnectionPtr& conn);
  void removeConnectionInLoop(const TcpConnectionPtr& conn);
  void newConnection(int sockfd, const Address& peerAddr);
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
  EventLoop* loop_;
  const std::string name_;
  bool started_;
  int nextConfd_;
  std::unique_ptr<Acceptor> acceptor_;
  std::unique_ptr<threadLoopPool> threadLoopPool_;
  ConnectionCallBack connectionBack_;
  MessageCallBack messageBack_;
  CloseCallBack closeCallBack_;
  ConnectionMap connection_;
  threadFuncType threadCallback_;
};

#endif