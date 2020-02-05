#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "modelHead.h"
#include "Epoll.h"

#include <sys/eventfd.h>

class Channel;
// class Poller;

// extern __thread EventLoop* loopInThisThread;

class EventLoop {
    public:
        EventLoop() : looping_(false),
                        quit_(false),
                        eventHanding_(false),
                        // poller_(Poller::newDefaultPoller(this)) 
                        poller_(new _Poller::Poller())
                        // threadId_(currentThread::tid()),
                        // wakeFd_(createEventFd()),
                        // wakeupChannel_(new Channel(this,wakeFd_)),
                        // callingPendingFunctors_(false)
{
    // if(!loopInThisThread) 
    //     loopInThisThread = this;
}
        ~EventLoop() {
            // assert(!looping_);
            // loopInThisThread = nullptr;
        }
        typedef std::function <void()> lifeCycle;
        typedef std::function <void()> Functor;
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);
        void runInLoop(const CallBack& cb);
        // bool isInLoopThread() { return threadId_ == currentThread::tid(); }
        // void assertInLoopThread();
        void loop();
        void quit();
        // void queueInLoop(const Functor& cb);
        
        // static int upFd;
                
    private:
        bool looping_;
        bool quit_;
        bool eventHanding_;
        // const pid_t threadId_;
        
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
       
        std::shared_ptr <_Poller::Poller> poller_;
        // void abortNotInLoopThread();

        // static EventLoop* getEventLoopOfThread();

        // void handleRead();
        // void doPendingFunctor();
        // void wakeup();
        // int createEventFd();
        // bool callingPendingFunctors_;
        // int wakeFd_;
        // std::unique_ptr <Channel> wakeupChannel_;
        // std::vector <Functor> pendingsFunctors_;
        // _Mutex::Mutex mutex_;

};
#endif