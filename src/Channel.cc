#include "Channel.h"

#include "EventLoop.h"
const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = EPOLLIN;
const int Channel::WriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      tied_(false),
      eventHanding_(false) {}

void Channel::update(void) { loop_->updateChannel(this); }

void Channel::handleEvents() {
  eventHanding_ = true;
  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    if (closeCallBack_) closeCallBack_();

  if (revents_ & EPOLLERR) {
    if (errCallBack_) closeCallBack_();
  }
  if (revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
    if (readCallBack_) readCallBack_();
  }
  if (revents_ & EPOLLOUT) {
    if (writeCallBack_) writeCallBack_();
  }
  eventHanding_ = false;
}
void Channel::handleEvent() {
  shared_ptr<void> t = tie_.lock();
  if (tied_) {
    if (t) {
      handleEvents();
    }
  } else {
    handleEvents();
  }
}
void Channel::remove(void) {
  assert(isNoneEvent());
  loop_->removeChannel(this);
}
void Channel::tie(const std::shared_ptr<void>& obj) {
  tie_ = obj;
  tied_ = true;
}
Channel::~Channel() {
  assert(!eventHanding_);
  this->disableAll();
  this->remove();
  ::close(fd_);
}