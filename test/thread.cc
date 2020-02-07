#include <iostream>
#include <thread>
#include "../src/EventLoop.h"
void threadFunc() {
    EventLoop loop;
    loop.loop();
}
EventLoop* loop2;
void threadFunc2() {
    loop2->loop();
}
void EventLoop::loop() {
    assert(!looping_);
    looping_ = true;
    ::poll(NULL,0,3*1000);
    looping_ = false;
}
int main (void) {
    EventLoop loop;
    loop2 = &loop;
    thread t(threadFunc2);
    t.join();
    return 0;
}