#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "modelHead.h"
#include "Epoll.h"
#include <sys/eventfd.h>
#include <thread>
#include <mutex>

class Channel;
using namespace std;


class EventLoop {
    public:
        EventLoop();
        ~EventLoop();
        typedef std::function <void()> Functor;
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);
        
        void assertNotInLoopThread() {
            if(!isInLoopThread()) {cout << " not in Loop Thread " << endl; abort(); }
        }
        bool isInLoopThread();
        void loop();
        void quit();
        void runInLoop(const Functor& cb);
        void queueInLoop(const Functor& cb);
        //wake up
        void handleRead();
        void pendingFunctor();
        bool callFunctor_;
        int wkfd_;
        unique_ptr<Channel> wkChannel_;
        mutex mutex_;
        vector<Functor> pendingVector_;
        void wakeup();
        static EventLoop* getThisThread();
    private:
        bool looping_;
        bool quit_;
        bool eventHanding_;
        pid_t threadId_;
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
        std::shared_ptr <_Poller::Poller> poller_;
};
#endif