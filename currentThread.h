// #ifndef __CURRENT_THREAD_H_
// #define __CURRENT_THREAD_H_
// #include <iostream>
// #include <sys/syscall.h>
// #include <sys/prctl.h>
// #include <unistd.h>
// using namespace std;

// namespace currentThread
// {
//   extern __thread int t_cachedTid;
// //   extern __thread char t_tidString[32];
// //   extern __thread int t_tidStringLength;
// //   extern __thread const char* t_threadName;
//   pid_t gettid (); 

//   void cacheTid (); 
  
//   inline int tid() {
//         if (__builtin_expect(t_cachedTid == 0, 0))
//         {
//         cacheTid();
//         }
//         return t_cachedTid;
//     }
// //   inline const char* tidString() // for logging
// //   {
// //     return t_tidString;
// //   }

// //   inline int tidStringLength() // for logging
// //   {
// //     return t_tidStringLength;
// //   }

// //   inline const char* name()
// //   {
// //     return t_threadName;
// //   }
// }
// #endif