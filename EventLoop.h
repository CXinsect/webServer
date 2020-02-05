#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "modelHead.h"
#include "Epoll.h"

#include <sys/eventfd.h>

class Channel;

class EventLoop {
    public:
        EventLoop() : looping_(false),
                        quit_(false),
                        eventHanding_(false),
                        poller_(new _Poller::Poller())
                      
{
   
}
        ~EventLoop() {
        }
        typedef std::function <void()> lifeCycle;
        typedef std::function <void()> Functor;
        void updateChannel(Channel * channel);
        void removeChannel(Channel * channel);
        void loop();
        void quit();
    private:
        bool looping_;
        bool quit_;
        bool eventHanding_;
        
        typedef std::vector<Channel *> ChannelList;
        ChannelList activeChannels_;
        std::shared_ptr <_Poller::Poller> poller_;

};
#endif