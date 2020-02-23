#include "ThreadLoopPool.h"

#include "EventLoop.h"

EventLoop *threadLoop::startLoop() {
  {
    unique_lock<mutex> mylock(mutex_);
    thread_.push_back(thread(&threadLoop::threadFunction, this));
    while (loop_ == NULL) cond_.wait(mylock);
  }
  // cout << "thread:::_____________" << loop_ << endl;
  return loop_;
}

void threadLoop::threadFunction() {
  EventLoop loop;
  if (callback_) callback_(&loop);
  {
    unique_lock<mutex> mylock(mutex_);
    loop_ = &loop;
    cond_.notify_one();
  }
  loop.loop();
}

void threadLoopPool::start(const threadFuncType &cb) {
  assert(!started_);
  bloop_->assertNotInLoopThread();
  started_ = true;
  for (int i = 0; i < numThreads_; i++) {
    threadLoop *t = new threadLoop(cb);
    loopv_.push_back(t->startLoop());
  }
  if (numThreads_ == 0 && cb) cb(bloop_);
}
EventLoop *threadLoopPool::getNextloop() {
  assert(started_);
  bloop_->assertNotInLoopThread();
  EventLoop *loop = bloop_;
  if (!loopv_.empty()) {
    // cout << "index--::loop" << endl;
    loop = loopv_[index_++];
    if (index_ >= loopv_.size()) index_ = 0;
  }
  return loop;
}