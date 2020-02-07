#ifndef _MODEL_HEAD_H_
#define _MODEL_HEAD_H_
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <assert.h>
#include <boost/any.hpp>
#include <boost/circular_buffer.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <sys/eventfd.h>

class TcpConnection;
class Buffer;
class EventLoop;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallBack;
typedef std::function<void(const TcpConnectionPtr&, Buffer* buf)>MessageCallBack;

typedef std::function<void(const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void(const TcpConnectionPtr&)> threadFuncCallBack;
typedef std::function<void(EventLoop*)> threadFuncType;
typedef std::function<void()> CallBack;
void defaultMessageCallback(const TcpConnectionPtr&, Buffer* buf);
void defaultConnectionCallback(const TcpConnectionPtr& conn);

class SignalPipe {
 public:
  SignalPipe() { ::signal(SIGPIPE, SIG_IGN); }
};
#endif