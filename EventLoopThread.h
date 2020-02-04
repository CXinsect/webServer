// #include "Mutex.h"
// #include "EventLoop.h"
// #include "currentThread.h"

// #include <iostream>
// #include <thread>
// #include <unistd.h>
// #include <boost/function.hpp>
// #include <boost/bind.hpp>

// using namespace _Mutex;
// // using namespace currentThread;

// typedef boost::function<void(EventLoop*)> eventLoopThreadCallBack;

// typedef boost::function<void()> threadCallBack;

// namespace _eventLoopThread {
    
//     pid_t getTid();
    
//     struct MutexFetch {
//         Mutex mutex_;
//         Cond cond_;
//         int count_;
//         void Wait();
//         void Signal();
//         MutexFetch(int count);
//     };

//     struct ThreadData {
//         MutexFetch fetch_;
//         pid_t* tid_;
//         threadCallBack callback_;
//         void runThreadFunc();
//         ThreadData(pid_t* tid,threadCallBack cb,MutexFetch&);
//     };
    
//     void* threadFunc(void*);
//     struct Thread {
//        Mutex mutex_;
//        Cond cond_;
//        pid_t tid_;
//        threadCallBack callback_;
//        pthread_t pid_;
//        bool join_;
//        MutexFetch fetch_;
//        void Start();
//        bool Join();
//        Thread(threadCallBack  cb);
//        ~Thread();
//    };

//     class eventLoopThread {
//         public:
//             eventLoopThread(eventLoopThreadCallBack const& cb = eventLoopThreadCallBack());
//             ~eventLoopThread();
//             EventLoop* startLoop();
//         private:
//         //线程的执行函数
//             void threadFunc();
            
//         private:
//             EventLoop* loop_;
//             bool exit_;
//             Thread thread_;
//             Mutex mutex_;
//             Cond cond_;
//             eventLoopThreadCallBack tcallBack_;
//             pid_t tid_;
//     };
  
// }