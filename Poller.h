#ifndef _POLLER_H_
#define _POLLER_H_

#include "modelHead.h"
#include "EventLoop.h"
class Channel;
struct pollfd;
class EventLoop;

class Poller {
    public:
        typedef std::vector <Channel *> ChannelList;
        Poller () {}
        Poller(EventLoop *loop);
         ~Poller();
        void poll(int timeout,ChannelList *activeChannels);
        void updateChannel(Channel *channel);
        void removeChannel(Channel *channel);
        
    private:
        void fillActiveChannels(int numEvents,ChannelList *activeChannels) const;
        
        typedef std::map<int,Channel *> ChannelMap;
        typedef std::vector <struct pollfd> PollList;
        ChannelMap channels_;
        PollList pollfds_;

};
#endif