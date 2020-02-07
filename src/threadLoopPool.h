#ifndef __LOOP_THREAD_H_
#define __LOOP_THREAD_H_
#include "modelHead.h"
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>
class EventLoop;
using namespace std;


class threadLoop {
    public:
        threadLoop(const threadFuncType& cb) :loop_(nullptr),callback_(cb) {}
        ~threadLoop() {
            for(int i = 0;i < thread_.size();i++) {
                thread_[i].join();
            }
        }
        EventLoop* startLoop();
        //begin function
        void threadFunction();
    private:
        EventLoop* loop_;
        mutex mutex_;
        condition_variable cond_;
        threadFuncType callback_;
        vector<thread> thread_;

};   

class threadLoopPool {
    public:
        threadLoopPool(EventLoop* loop) : bloop_(loop),started_(false),numThreads_(0),index_(0) {}
        ~threadLoopPool() {};
        void setThreadnums(int nums) { numThreads_ = nums; }
        void start(const threadFuncType& cb);
        EventLoop* getNextloop();
    private:
        EventLoop* bloop_;
        bool started_;
        int index_;
        int numThreads_;
        vector<EventLoop*> loopv_;
};
#endif