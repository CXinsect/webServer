#include "EventLoop.h"
#include "Channel.h"
#include "currentThread.h"

int createWakeupfd () {
  int efd = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
  assert(efd >= 0);
  return efd;
}

__thread EventLoop* loopThisThread = 0;

using namespace std;

EventLoop::EventLoop() :looping_(false),
                        quit_(false),
                        eventHanding_(false),
                        poller_(new _Poller::Poller(this)),
                        wkfd_(createWakeupfd()),
                        wkChannel_(new Channel(this,wkfd_)),
                        threadId_(CurrentThread::tid())
{
        if(loopThisThread) {
            cout << "not in this loop" << endl;
        } else {
            loopThisThread = this;
        }
        wkChannel_->setReadCallBack(bind(&EventLoop::handleRead,this));
        wkChannel_->enableReading();
}
EventLoop::~EventLoop() { 
    wkChannel_->disableAll();
    wkChannel_->remove();
    close(wkChannel_->getFd());
    loopThisThread = NULL;
}
bool EventLoop::isInLoopThread() { return threadId_ == CurrentThread::tid(); }

EventLoop* EventLoop::getThisThread() { return loopThisThread; }

void EventLoop::loop() {
    assert(!looping_);
    assertNotInLoopThread();
    looping_ = true;
    quit_ = false;
    while(!quit_) {
        std::cout << "Loop" << std::endl;
        activeChannels_.clear();
        poller_->poll(&activeChannels_,-1);
        for(ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end();it++) {
                (*it)->handleEvent();
            }
        pendingFunctor();
    }
    std::cout << "Event is going to be Stoped" << std::endl;
    looping_ = false;
}
void EventLoop::updateChannel(Channel * channel) {
    poller_->updateChannel(channel);
}
void EventLoop::quit() {
    quit_ = true;
    if(!isInLoopThread()) wakeup();
}
void EventLoop::queueInLoop (const Functor& cb) {
    unique_lock<mutex> mylock(mutex_);
    pendingVector_.push_back(cb);
    mylock.unlock();
    if(!isInLoopThread() || callFunctor_) {
        //wake up
        wakeup();
    }

}
void EventLoop::wakeup() {
    int flag = 1;
    int ret = ::write(wkfd_,&flag,sizeof flag);
    assert(ret != sizeof flag);
}
void EventLoop::handleRead() {
    int flag = 1;
    int ret = ::read(wkfd_,&flag,sizeof flag);
    assert(ret != sizeof flag);
}
void EventLoop::pendingFunctor() {
    vector<Functor> tmp;
    callFunctor_ = true;
    mutex_.lock();
    tmp.swap(pendingVector_);
    mutex_.unlock();
    for(int i = 0;i < tmp.size();i++) {
        tmp[i]();
    }
    callFunctor_ = false;
}
void EventLoop::runInLoop(const Functor& cb) {
    if(isInLoopThread()) cb();
    else queueInLoop(cb);
}
void EventLoop::removeChannel(Channel * channel_) {
    assert(channel_->owerLoop() == this);
    if(eventHanding_) {
        assert(std::find(activeChannels_.begin(),activeChannels_.end(),channel_) != activeChannels_.end());
    }
    poller_->removeChannel(channel_);
}