#include "../src/EventLoop.h"

EventLoop* t;
void threadFunc () {
    t->loop();
}

int main (void) {
    EventLoop loop;
    t = &loop;
    thread t(threadFunc);
    t.join();
}
