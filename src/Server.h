#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <memory>
#include <unordered_set>

#include "Address.h"
#include "EventLoop.h"
#include "SocketOpts.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "disCription.h"
#include "modelHead.h"
#include "WebRequest.h"
#include "WebResponse.h"

class ThreadPool;

class Server {
 public:
  typedef std::function<void(const webRequest &)> webCallBack;

  Server(EventLoop *loop, const Address &addr, const string &name);
  ~Server();
  void setCallBack(const webCallBack &cb) { webcallback_ = cb; }
  void start() {
    server_.setThreadNums(4);
    server_.start();
  }
  void Run();

 private:
  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf);
  void onRequest(const TcpConnectionPtr &, disCription::HttpCode &status,
                 FastCGI &fastcgi_);

 private:
  TcpServer server_;
  webCallBack webcallback_;
  EventLoop *loop_;
  FastCGI fastcgi_;
};

#endif