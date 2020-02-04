// #ifndef __MUTEX_H_
// #define __MUTEX_H_
// #include <iostream>
// #include <pthread.h>
// #include <assert.h>
// #include "currentThread.h"
// namespace _Mutex {
//     class Mutex {
//         public:
//             Mutex() :holder_(0) {
//                 if(pthread_mutex_init(&mutex_,NULL) != 0) {
//                     throw std::exception();
//                 }
//             }
//             ~Mutex() {
//                 assert(holder_ == 0);
//                 pthread_mutex_destroy(&mutex_);
//             }
//             bool isLockInThisThread() { return holder_ == currentThread::tid(); }
          
//             void assertNotInThisThread() { assert(isLockInThisThread()); }
          
//             void lock() {
//                 pthread_mutex_lock(&mutex_);
//                 holder_ = currentThread::tid();
//             }
//             void unlock() {
//                 holder_ = 0;
//                 pthread_mutex_unlock(&mutex_);
//             }


//         private:
//             pthread_mutex_t mutex_;
//             pid_t holder_;
//     };
//     class Cond {
//         public:
//             Cond() {
//                 if(pthread_mutex_init(&mutex_,NULL) != 0)
//                     throw std::exception();
                
//                 if(pthread_cond_init(&cond_,NULL) != 0) {
//                     pthread_mutex_destroy(&mutex_);
//                     throw std::exception();
//                 }
//             }
//             ~Cond() {
//                 pthread_mutex_destroy(&mutex_);
//                 pthread_cond_destroy(&cond_);
//             }
//             bool wait() {
//                 int ret = 0;
//                 pthread_mutex_lock(&mutex_);
//                 pthread_cond_wait(&cond_,&mutex_);
//                 pthread_mutex_unlock(&mutex_);
//                 return ret == 0;
//             }
//             bool signalSingle() {
//                 return pthread_cond_signal(&cond_) == 0;
//             }
//             bool signalAll() {
//                 return pthread_cond_broadcast(&cond_) == 0;
//             }
//         private:
//             pthread_mutex_t mutex_;
//             pthread_cond_t cond_;
//     };
// }
// #endif