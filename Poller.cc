#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop) : owerLoop_(loop) {}

Poller::~Poller() {std::cout << "ddd" << std::endl;};

void Poller::poll(int timeoutMs,ChannelList *activeChannels) {
    std::cout << "Comming in Poll" << std::endl;
    std::cout << "pollsize" << pollfds_.size() << std::endl;    
    int numEvents = ::poll(&*pollfds_.begin(),pollfds_.size(),5);
    if(numEvents > 0) {
        std::cout << "active Channels" << numEvents << std::endl;
        fillActiveChannels(numEvents,activeChannels);
    }
    else if(numEvents == 0) {
        std::cout << "Nothing happend" << std::endl;
    }
    else {
        std::cout << __FILE__ << ": " << __LINE__ << std::endl;
        exit(0);
    }
    return;
}
void Poller::fillActiveChannels (int numEvents,ChannelList *activeChannels) const {
    for(PollList::const_iterator pfd = pollfds_.begin();
        pfd != pollfds_.end() && numEvents > 0;++pfd) {
            if(pfd->revents > 0) {
                --numEvents;
                ChannelMap::const_iterator ch = channels_.find(pfd->fd);
                assert(ch != channels_.end());
                Channel * channel = ch->second;
                assert(channel->getFd() == pfd->fd);
              //更新events状态
                channel->setRevents(pfd->revents);
                activeChannels->push_back(channel);
            }
        }
}

void Poller::updateChannel(Channel *channel) {
    std::cout << "fd= " << channel->getFd() << "events= " << channel->getEvents() << std::endl;
    //如果是新事件则加入事件列表
    if(channel->getIndex() < 0) {
        std::cout << "First coming updateChannnel " << std::endl;
        assert(channels_.find(channel->getFd()) == channels_.end());
        struct pollfd pfd;
        pfd.fd = channel->getFd();
        pfd.events = static_cast<short>(channel->getEvents());
        pfd.revents = 0;
        pollfds_.push_back(pfd);
        int id = static_cast <int>(pollfds_.size()) -1;
        std::cout << "id: " << id << std::endl;
        channel->setIndex(id);
        channels_[pfd.fd] = channel;
        
    }
    else {
        //否则则更新描述符信息
        std::cout << "update exist Channel " << std::endl;
        assert(channels_.find(channel->getFd()) != channels_.end());
        assert(channels_[channel->getFd()] == channel);
        int id = channel->getIndex();
        assert(id >= 0 && id < static_cast<int>(pollfds_.size()));
        struct pollfd & pfd = pollfds_[id];
        assert(channel->getFd() == pfd.fd || pfd.fd == -channel->getFd()-1);
        pfd.events = static_cast<short>(channel->getEvents());
        pfd.revents = 0;
        if(channel->isNoneEvent()) {
            pfd.fd = -channel->getFd()-1;
        }
            // pfd.fd = -1; 
    }
}
void Poller::removeChannel(Channel *channel) {
    std::cout << "Poller::removeChannel: " << channel->getFd() << std::endl;
    assert(channels_.find(channel->getFd()) != channels_.end());
    assert(channels_[channel->getFd()] == channel);
    assert(channel->isNoneEvent());
    int id = channel->getIndex();
    assert(id >= 0 && id < static_cast<int>(pollfds_.size()));
    const struct pollfd &pfd = pollfds_[id];
    // std::cout << "Poller::remove" << channel->getFd() << channel->getEvents() << std::endl;
    assert(pfd.fd == -channel->getFd()-1 && pfd.events == channel->getEvents());
    size_t n = channels_.erase(channel->getFd());
    assert(n == 1);
    if(boost::implicit_cast<size_t>(id) == pollfds_.size()-1) {
        pollfds_.pop_back();
    }
    else {
        int channelEnd = pollfds_.back().fd;
        iter_swap(pollfds_.begin()+id,pollfds_.end()-1);
        if(channelEnd < 0) {
            channelEnd = -channelEnd-1;
        }
        channels_[channelEnd]->setIndex(id);
        pollfds_.pop_back();
    }

}
