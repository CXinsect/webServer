#include "Server.h"

#include <functional>
#include <iostream>

using namespace placeholders;
using namespace std;

Server::Server(EventLoop *loop, const Address &listenAddr, const string &name)
    : server_(loop, listenAddr, name), loop_(loop) {
  fastcgi_.FastCgi_init();
  server_.setConnectionCallBack(std::bind(&Server::onConnection, this, _1));
  server_.setMessageCallBack(std::bind(&Server::onMessage, this, _1, _2));
}
Server::~Server() {}
void Server::onConnection(const TcpConnectionPtr &conn) {
  if (conn->isConnected()) {
    std::cout << "New Connection " << conn->getName() << std::endl;
    std::shared_ptr<webRequest> ptr_(new webRequest);
    conn->setRequest(ptr_);
  }
}
void Server::onMessage(const TcpConnectionPtr &conn, Buffer *buf) {
  std::shared_ptr<webRequest> request_ = conn->getRequest();
  request_->setBuffer(*buf);
  std::cout << "bufer::onMessage " << buf->retrieveAllAsString() << std::endl;
  disCription::HttpCode ret = request_->eventProcess(fastcgi_);
  onRequest(conn, ret, fastcgi_);
}
void Server::onRequest(const TcpConnectionPtr &conn,
                       disCription::HttpCode &status, FastCGI &fastcgi_) {
  unique_ptr<Buffer> buffer_(new Buffer);
  webResponse response_;
  response_.setHttpCodeStatus(status);
  bool flags = response_.fileResponseAssembly(buffer_);
  do {
    if (!flags) {
      conn->handClose();
      break;
    }
    conn->send(buffer_->retrieveAllAsString());
  } while (0);
  conn->connectionClose();
}
