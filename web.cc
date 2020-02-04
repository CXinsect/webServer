#include "Server.cc"

int main (void) {
    EventLoop loop;
    SignalPipe signalPipe;
    Address listenAddr(8888);
    Server server(&loop,listenAddr,"WebServer",500);
    server.start();
    loop.loop();
}