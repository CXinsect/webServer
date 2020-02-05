#include "threadPool.h"
// #include "Buffer.h"
#include "Server.h"

using namespace placeholders;

void ThreadPool::processingTask() {
    while(!isStoped()) {
      unique_lock<mutex> mylock(mutex_);
      cond_.wait(mylock, [this]{ if(queue_.empty() && !isStoped()) return false; else return true; });
      if(queue_.empty()) continue;
      auto task = queue_.front();
      queue_.pop();
      mylock.unlock();
      callback(task);
    }
}

void ThreadPool::addTask (const TcpConnectionPtr& task) {
    unique_lock<mutex> mylock(mutex_);
    queue_.push(task);
    cond_.notify_one();
}

void ThreadPool::Start() {
    cout << "create thread" << endl;
    for(int i = 0;i < numThreads_;i++) {
        v.push_back(thread(&ThreadPool::processingTask,this));
    }
}
void ThreadPool::Stop() {
    stoped_ = true;
    cond_.notify_all();
    for(auto& t : v) {
        t.join();
    }
    mutex_.lock();
    while(!queue_.empty()) {
        // Task* task = queue_.front();
        queue_.pop();
        // delete task;
    }
    mutex_.unlock();
}