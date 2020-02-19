#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <functional>

#include "modelHead.h"

class EventLoop;

class Channel {
 public:
  typedef std::function<void()> EventCallBack;
  Channel(EventLoop *loop, int fd);

  void handleEvent();
  void handleEvents();
  void setReadCallBack(const EventCallBack &cb) { readCallBack_ = cb; }
  void setWriteCallBack(const EventCallBack &cb) { writeCallBack_ = cb; }
  void setErrCallBack(const EventCallBack &cb) { errCallBack_ = cb; }
  void setCloseCallBack(const EventCallBack &cb) { closeCallBack_ = cb; }
  void setForceCloseCallBack(const EventCallBack &cb) { forceCallBack_ = cb; }
  int getFd() { return fd_; }
  void setFd(int fd) { fd_ = fd; }
  int getEvents() const { return events_; }
  void setRevents(int revt) { revents_ = revt; }
  bool isNoneEvent() const { return events_ == NoneEvent; }
  bool isWriteing() const { return events_ & WriteEvent; }
  void enableReading() {
    events_ |= ReadEvent;
    update();
  }
  void enableWriteing() {
    events_ |= WriteEvent;
    update();
  }
  void disableWriteing() {
    events_ &= ~WriteEvent;
    update();
  }
  void disableAll() {
    events_ = NoneEvent;
    update();
  }
  int getIndex() { return index_; }

  void setIndex(int id) { index_ = id; }
  EventLoop *owerLoop() { return loop_; }
  void remove(void);
  void tie(const std::shared_ptr<void> &);
  ~Channel();

 private:
  void update();
  static const int NoneEvent;
  static const int ReadEvent;
  static const int WriteEvent;
  EventLoop *loop_;
  int fd_;
  //事件集合
  int events_;
  //活跃事件集合
  int revents_;
  int index_;
  //设置回调
  EventCallBack readCallBack_;
  EventCallBack writeCallBack_;
  EventCallBack errCallBack_;
  EventCallBack closeCallBack_;
  EventCallBack forceCallBack_;
  bool eventHanding_;
  std::weak_ptr<void> tie_;
  bool tied_;
};
#endif