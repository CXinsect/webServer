// #ifndef __MUTEX_H_
// #define __MUTEX_H_
// #include <iostream>
// #include <pthread.h>
// #include <assert.h>
// #include "currentThread.h"
// namespace _Mutex {
//     class Mutex {
//         public:
//             Mutex();

//             ~Mutex();

//             bool isLockInThisThread();
          
//             void assertNotInThisThread();
          
//             void lock();

//             void unlock();

//         private:
//             pthread_mutex_t mutex_;
//             pid_t holder_;
//     };
//     class Cond {
//         public:
//             Cond();
//             Cond(pthread_mutex_t& mutex) : mutex_(mutex) {};
//             ~Cond();

//             bool wait();

//             bool signalSingle();
//             bool signalAll();    
      
//         private:
//             pthread_mutex_t mutex_;
//             pthread_cond_t cond_;
//     };
//     class MutexGuard {
//         public:
//             explicit MutexGuard(Mutex& mutex) :mutex_(mutex)
//             {
//                 mutex_.lock();
//             }
//             ~MutexGuard() {
//                 mutex_.unlock();
//             }
//         private:
//             Mutex mutex_;
//     };
// }
// #endif