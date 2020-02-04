#include "Channel.h"
#include "EventLoop.cc"
#include "Poller.h"
#include <sys/timerfd.h>

EventLoop * loop_;
void timeout() {
    std::cout << "timeout" << std::endl;
    loop_->quit();
    // exit(0);
}
int main (void) {
    EventLoop loop;
    loop_ = &loop;
    int termfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    Channel channel(&loop,termfd);
    channel.setReadCallBack(timeout);
     channel.enableReading();
    struct itimerspec howlong;
    bzero(&howlong,sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(termfd,0,&howlong,NULL);
     loop.loop();
     ::close(termfd);
    return 0;
}