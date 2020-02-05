#ifndef __EPOLL_H_
#define __EPOLL_H_
#include "modelHead.h"
class Channel;
class EventLoop;
namespace _Poller {

    class Poller {
        public:
            Poller() {
                epollfd_ = epoll_create(10000);
            };
            Poller(EventLoop* loop) {
                
            }
            ~Poller() { ::close(epollfd_); }
            typedef std::vector<Channel*> channelList;
            void poll (channelList* activeChannelList,int timeout);
            void updateChannel(Channel* channel);
            void removeChannel(Channel* channel);
        
        private:
            
            void fillActiveChannels(int numEvents,channelList* activeChannels) const;
            typedef std::map<int,Channel*> channelMap;
            typedef std::vector <struct epoll_event> eventList;

            channelMap channels_;
            eventList events_;
            int epollfd_;

    };
}
#endif
