#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"
void _Poller::Poller::fillActiveChannels (int numEvents,channelList* activeChannels) const {
    assert(numEvents <= events_.size());
    std::cout << "Coming Epoll" << std::endl;
    for(int i = 0;i < numEvents; i++) {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        int fd =  channel->getFd();
        channelMap::const_iterator it = channels_.find(fd);
        assert(it != channels_.end());
        assert(it->second == channel);
        channel->setRevents(events_[i].events);
        activeChannels->push_back(channel);
    }
}

void _Poller::Poller::updateChannel(Channel* channel) {
    if(channel->getIndex() < 0) {
        assert(channels_.find(channel->getFd()) == channels_.end());
        struct epoll_event efd;
        efd.data.ptr = static_cast<void*>(channel);
        efd.events = static_cast<short>(channel->getEvents());
        assert(epoll_ctl(epollfd_,EPOLL_CTL_ADD,channel->getFd(),&efd) != -1);
        events_.push_back(efd);
        int index = static_cast<int>(events_.size() - 1);
        channel->setIndex(index);
        channels_[channel->getFd()] = channel;

    } else {
        assert(channels_.find(channel->getFd()) != channels_.end());
        assert(channels_[channel->getFd()] == channel);
        int index = channel->getIndex();
        assert(index >= 0 && index < static_cast<int>(events_.size()));
        struct epoll_event& efd = events_[index];
        assert(static_cast<Channel*>(efd.data.ptr)->getFd() == channel->getFd() || static_cast<Channel*>(efd.data.ptr) ->getFd() == -channel->getFd()-1);
        efd.data.ptr = static_cast<void*>(channel);
        efd.events = static_cast<short>(channel->getEvents());
        if(channel->isNoneEvent()) {
            Channel* tmp = static_cast<Channel*>(efd.data.ptr);
            int tfd = channel->getFd();
        }
        assert(epoll_ctl(epollfd_,EPOLL_CTL_MOD,channel->getFd(),&efd) != -1);

    }
}

void _Poller::Poller::removeChannel(Channel* channel) {
    assert(channels_.find(channel->getFd()) != channels_.end());
    assert(channels_[channel->getFd()] == channel);
    assert(channel->isNoneEvent());
    int index = channel->getIndex();
    assert(index >= 0 && index < static_cast<int>(events_.size()));
    const struct epoll_event& efd = events_[index];
    (void)efd;
    assert(efd.data.ptr == NULL || efd.events == channel->getEvents());
    size_t n = channels_.erase(channel->getFd());
    assert(n == 1);
    if(static_cast<size_t>(index) == events_.size() - 1) {
        events_.pop_back();
    } else {
        Channel *end = static_cast<Channel*>(events_.back().data.ptr);
        iter_swap(events_.begin() + index,events_.end() - 1);
        channels_[end->getFd()]->setIndex(index);
        events_.pop_back();
    }
    assert(epoll_ctl(epollfd_,EPOLL_CTL_DEL,channel->getFd(),const_cast<epoll_event*>(&efd)) != -1);
    
}

void _Poller::Poller::poll (channelList* activeChannel,int timeout) {
    assert(epollfd_ > 0);
    std::cout <<"Hello" << std::endl;
    int numEvents = ::epoll_wait(epollfd_,&*events_.begin(),static_cast<int>(events_.size()),timeout);
    if(numEvents > 0) {
        fillActiveChannels(numEvents,activeChannel);
        if(static_cast<size_t>(numEvents) == events_.size()) {
            events_.resize(events_.size() * 2);
        }
    }
}