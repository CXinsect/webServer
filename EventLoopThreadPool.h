// #include "EventLoop.h"
// #include "Mutex.h"
// #include <iostream>
// #include <thread>
// #include <assert.h>
// #include <boost/ptr_container/ptr_vector.hpp>
// #include <boost/function.hpp>

// using namespace _Mutex;

// class EventLoopThread;

// namespace _eventThreadPool {

//     typedef boost::function<void(EventLoop*)> threadCallBack;

//     class eventThreadPool {
//         public:
//             eventThreadPool(EventLoop* baseLoop);
//             ~eventThreadPool() =default;
//             void setThreadNum(int numThreads);
//             EventLoop* getLoop();
//             void Start(threadCallBack const& cb = threadCallBack());

//         private:
//             EventLoop* baseLoop_;
//             bool started_;
//             int numThreads_;
//             int index_;
//             // Mutex mutex_;
//             // Cond cond_;
//             std::vector<EventLoop*> loop_;
//             boost::ptr_vector<EventLoopThread> threads_;
//     };
// }