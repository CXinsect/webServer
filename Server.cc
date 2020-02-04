#include "Server.h"
#include <functional>
#include <iostream>
const std::_Placeholder<1> std::placeholders::_1;
const std::_Placeholder<2> std::placeholders::_2;

Server::Server(EventLoop* loop, const Address& listenAddr, const string& name,int timeSeconds)
    : server_(loop, listenAddr, name), loop_(loop), connectionBuckets_(timeSeconds) {
      
  fastcgi_.FastCgi_init();
  server_.setConnectionCallBack(boost::bind(&Server::onConnection, this, _1));
  server_.setMessageCallBack(boost::bind(&Server::onMessage, this, _1, _2));
  //过期时间处理函数
  connectionBuckets_.resize(timeSeconds);
}
void Server::onConnection(const TcpConnectionPtr& conn) {
  if (conn->isConnected()) {
    std::cout << "New Connection "
              << "[name: " << conn->getName() << std::endl;
    std::shared_ptr<webRequest> ptr_(new webRequest);
    conn->setRequest(ptr_);
    entryPtr_ entry(new Entry(conn));
    connectionBuckets_.back().insert(entry);
    entryWeakPtr_ weakEntry(entry);
    conn->setContext(weakEntry);
  } else {
    assert(!conn->getContext().empty());
    try {
      entryWeakPtr_ weakEntry(boost::any_cast<entryWeakPtr_>(conn->getContext()));
      std::cout << "Entry Use Count " << weakEntry.use_count() << std::endl;
    } catch(boost::bad_any_cast const& e) {
      std::cout << "Ooops: " << e.what() << std::endl;
    }
  }
}
void Server::onMessage(const TcpConnectionPtr& conn, Buffer* buf) {
  std::shared_ptr<webRequest> request_ = conn->getRequest();
  request_->setBuffer(*buf);
  std::cout << "bufer::onMessage " << buf->retrieveAllAsString() << std::endl;
  disCription::HttpCode ret = request_->eventProcess(fastcgi_);
  onRequest(conn, ret, fastcgi_);
  assert(!conn->getContext().empty());
  entryWeakPtr_ weakEntry(boost::any_cast<entryWeakPtr_>(conn->getContext()));
  entryPtr_ entry(weakEntry.lock());
  if(entry) {
    connectionBuckets_.back().insert(entry);
  }
}

void Server::onRequest(const TcpConnectionPtr& conn,
                       disCription::HttpCode& status, FastCGI& fastcgi_) {
  Buffer* buffer_ = new Buffer;
  webResponse response_;
  response_.setHttpCodeStatus(status);
  bool flags = response_.fileResponseAssembly(buffer_, fastcgi_);
  // std::cout << "Send Data: " << buffer_->retrieveAllAsString() << std::endl;
  
  do {
      if(!flags) {
          std::cout << "<<<<<flags error" << std::endl;
          conn->connectionClose();
          break;
      }
        conn->send(buffer_);
  }while(0);

//   conn->connectionClose();
}
