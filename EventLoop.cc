#include "EventLoop.h"
#include "Channel.cc"

// __thread EventLoop* loopInThisThread = 0;

#include "Poller.cc"

void EventLoop::loop() {
    assert(!looping_);
    // assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while(!quit_) {
        std::cout << "Loop" << std::endl;
        activeChannels_.clear();
        poller_->poll(&activeChannels_,-1);
        eventHanding_ = true;
        for(ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end();it++) {
                (*it)->handleEvent();
            }
        // doPendingFunctor();
        eventHanding_ = false;
    }
    std::cout << "Event is going to be Stoped" << std::endl;
    looping_ = false;
}
void EventLoop::updateChannel(Channel * channel) {
    assert(channel->owerLoop() == this);
    // assertInLoopThread();
    poller_->updateChannel(channel);
}
void EventLoop::quit() {
    quit_ = true;
    // if(!isInLoopThread())
    //     wakeup();
}
void EventLoop::runInLoop(const CallBack &cb) {
    // if(isInLoopThread())
        cb();
    // else queueInLoop(cb);
}
void EventLoop::removeChannel(Channel * channel_) {
    assert(channel_->owerLoop() == this);
    if(eventHanding_) {
        assert(std::find(activeChannels_.begin(),activeChannels_.end(),channel_) != activeChannels_.end());
    }
    poller_->removeChannel(channel_);
}

// void EventLoop::queueInLoop(const Functor& cb) {
//     {
//         _Mutex::MutexGuard lock(mutex_);
//         pendingsFunctors_.push_back(cb);
//     }
//     if(!isInLoopThread() || callingPendingFunctors_)
//         wakeup();
// }


// void EventLoop::abortNotInLoopThread () {
//     std::cout << "Current Tid: " << currentThread::tid()
//     << "IO Thread Id: " << threadId_ << std::endl; 
// }
// void EventLoop::assertInLoopThread() {
//     if(!isInLoopThread()) {
//         abortNotInLoopThread();
//     }
// }
// EventLoop* EventLoop::getEventLoopOfThread () {
//     return loopInThisThread; 
// }
// void EventLoop::doPendingFunctor() {
//     std::vector<Functor> tmp;
//     callingPendingFunctors_ = true;
//     {
//         _Mutex::MutexGuard lock(mutex_);
//         tmp.swap(pendingsFunctors_);
//     }
//     for(const Functor& temp : tmp)
//         temp();

//     callingPendingFunctors_ = false;
// }

// int EventLoop::createEventFd () {
//     int efd = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
//     assert(efd >= 0);
//     return efd;
// }

// void EventLoop::wakeup () {
//     int one = 1;
//     ssize_t n = ::write(wakeFd_,&one,sizeof one);
//     assert(static_cast<int>(n) > 0);
// }
// void EventLoop::handleRead () {
//     int one = 1;
//     ssize_t n = ::read(wakeFd_,&one,sizeof one);
//     assert(n > 0);
// }
