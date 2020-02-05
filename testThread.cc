#include "threadPool.h"

int main (void) {
    ThreadPool t(2);
    t.Start();
    t.addTask(new Task());
    t.addTask(new Task());
    t.Stop();
    sleep(3);
    return 0;
}