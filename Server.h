#ifndef _SERVER_H_
#define _SERVER_H_
#include <iostream>
#include <unordered_set>
#include <memory>
#include "Address.h"
#include "EventLoop.h"
#include "SocketOpts.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "modelHead.h"
#include "disCription.h"
#include "webRequest.h"
#include "webResponse.h"

class Server {
 public:
  typedef std::function<void(const webRequest &)> webCallBack;

  Server(EventLoop *loop, const Address &addr, const string &name,int Time);

  void setCallBack(const webCallBack &cb) { webcallback_ = cb; }
  void start() { server_.start(); }

 private:
  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf);
  void onRequest(const TcpConnectionPtr &, disCription::HttpCode &status,
                 FastCGI &fastcgi_);
  TcpServer server_;
  webCallBack webcallback_;
  EventLoop *loop_;
  FastCGI fastcgi_;

  void onTime() {
    connectionBuckets_.push_back(bucket_());
  }
  typedef std::weak_ptr<TcpConnection> weakTcpConnectionPtr_;

  class Entry {
    public:
      explicit Entry (weakTcpConnectionPtr_ const& weakConn) : weakConnection_(weakConn) {}
      ~Entry() {
        TcpConnectionPtr tmp(weakConnection_.lock());
        if(tmp) {
          tmp->Close();
        }
      }
    public:
      weakTcpConnectionPtr_ weakConnection_;
  };
  public:
    typedef std::shared_ptr<Entry> entryPtr_;
    typedef std::weak_ptr<Entry> entryWeakPtr_;
    typedef std::unordered_set<entryPtr_> bucket_;
    typedef boost::circular_buffer<bucket_> connectionWeakList_;
    connectionWeakList_ connectionBuckets_;
};

#endif