// #include "EventLoopThreadPool.h"

// using namespace _eventThreadPool;

// eventThreadPool::eventThreadPool (EventLoop* baseLoop) : baseLoop_(baseLoop),
//     started_(false),
//     numThreads_(0),
//     index_(0)
// {
// }

// void eventThreadPool::Start(threadCallBack const& cb) {
//     baseLoop_->assertInLoopThread();
//     started_ = true;
//     for(int i = 0; i< numThreads_;i++) {
//         //创建线程
//     }
//     if(numThreads_ == 0 && cb) {
//         cb(baseLoop_);
//     }
// }

// EventLoop* eventThreadPool::getLoop() {
//     baseLoop_->assertInLoopThread();
//     assert(started_);
//     EventLoop* loop = baseLoop_;
//     if(!loop_.empty()) {
//         loop = loop_[index_++];
//         if(static_cast<size_t>(index_) == loop_.size())
//             index_ = 0;
//     }
//     return loop;
// }

