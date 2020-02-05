#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "modelHead.h"

class Buffer;

using namespace std;

class ThreadPool {
    public:
        ThreadPool(int nums) : numThreads_(nums) , stoped_(false) {}
        ~ThreadPool() {}
        void processingTask();
        void addTask(const TcpConnectionPtr& task);
        void Start();
        void Stop();
        bool isStoped() { return stoped_; }
        void setCallBack(const threadFuncCallBack& cb) {
            callback = cb;
        }
    private:
        int numThreads_;
        bool stoped_;
        queue <TcpConnectionPtr> queue_;
        mutex mutex_;
        condition_variable cond_;
        vector<thread> v;
        threadFuncCallBack callback;
};
#endif