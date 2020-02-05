#include "EventLoop.h"
#include "Channel.h"

// __thread EventLoop* loopInThisThread = 0;

#include "Poller.h"

void EventLoop::loop() {
    assert(!looping_);
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
        eventHanding_ = false;
    }
    std::cout << "Event is going to be Stoped" << std::endl;
    looping_ = false;
}
void EventLoop::updateChannel(Channel * channel) {
    poller_->updateChannel(channel);
}
void EventLoop::quit() {
    quit_ = true;
}
void EventLoop::removeChannel(Channel * channel_) {
    assert(channel_->owerLoop() == this);
    if(eventHanding_) {
        assert(std::find(activeChannels_.begin(),activeChannels_.end(),channel_) != activeChannels_.end());
    }
    poller_->removeChannel(channel_);
}