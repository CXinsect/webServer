#include "src/Server.h"

int main (void) {
    EventLoop loop;
    SignalPipe signalPipe;
    Address listenAddr(8888);
    Server server(&loop,listenAddr,"WebServer");
    server.start();
    loop.loop();
}