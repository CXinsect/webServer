#include "Server.h"
#include <functional>
#include <iostream>
#include "threadPool.h"

using namespace placeholders;
using namespace std;

Server::Server(EventLoop* loop, const Address& listenAddr, const string& name,int timeSeconds)
    : server_(loop, listenAddr, name), loop_(loop), connectionBuckets_(timeSeconds),pool_(new ThreadPool(2)) {
      
  fastcgi_.FastCgi_init();
  server_.setConnectionCallBack(std::bind(&Server::onConnection,this , _1));
  server_.setMessageCallBack(std::bind(&Server::onMessage, this, _1, _2));
  //过期时间处理函数
  connectionBuckets_.resize(timeSeconds);
  pool_->setCallBack(std::bind(&Server::ThreadFunc,this,_1));
  pool_->Start();
}
Server::~Server() { pool_->Stop(); }
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
  setBuffer(buf);
  pool_->addTask(conn);
  entryWeakPtr_ weakEntry(boost::any_cast<entryWeakPtr_>(conn->getContext()));
  entryPtr_ entry(weakEntry.lock());
  if(entry) {
    connectionBuckets_.back().insert(entry);
  }
}
void Server::ThreadFunc (const TcpConnectionPtr& conn) {
  cout << this_thread::get_id() << "DDDDD" << endl;
  std::shared_ptr<webRequest> request_ = conn->getRequest();
  request_->setBuffer(*buffer_);
  std::cout << "bufer::onMessage " << buffer_->retrieveAllAsString() << std::endl;
  disCription::HttpCode ret = request_->eventProcess(fastcgi_);
  onRequest(conn, ret, fastcgi_);
  assert(!conn->getContext().empty());
}
void Server::onRequest(const TcpConnectionPtr& conn,
                       disCription::HttpCode& status, FastCGI& fastcgi_) {
  unique_ptr<Buffer> buffer_(new Buffer);
  webResponse response_;
  response_.setHttpCodeStatus(status);
  bool flags = response_.fileResponseAssembly(buffer_);
  do {
      if(!flags) {
          conn->handClose();
          break;
      }
        conn->send(buffer_->retrieveAllAsString());
  }while(0);
}
