// #include "EventLoopThread.h"

// using namespace _eventLoopThread;

// pid_t _eventLoopThread::getTid() {
//     return static_cast<pid_t>(::syscall(SYS_gettid));
// }

// void currentThread::cacheTid() {
//     if(t_cachedTid == 0) {
//         t_cachedTid = getTid();
//     }
// }

// MutexFetch::MutexFetch(int count):
//     mutex_(),
//     cond_(mutex_),
//     count_(count)
// {
// }

// void MutexFetch::Wait() {
//     MutexGuard lock(mutex_);
//     while(count_ > 0) {
//         cond_.wait();
//     }
// }
// void MutexFetch::Signal() {
//     MutexGuard lock(mutex_);
//     count_--;

//     if(count_ == 0)
//         assert(cond_.signalAll());
// }

// Thread::Thread(threadCallBack cb)
//     :tid_(0),
//     callback_(std::move(cb)),
//     mutex_(),
//     cond_(mutex_),
//     join_(false),
//     fetch_(1)
// {
// }
// ThreadData::ThreadData(pid_t* tid,threadCallBack callback,MutexFetch& fetch)
//     :tid_(tid),
//     callback_(std::move(callback)),
//     fetch_(fetch)
// {
// }

// void ThreadData::runThreadFunc() {
//     *tid_ = currentThread::tid();
//     tid_ = NULL;
//     callback_();
// }
// void Thread::Start() {
//     ThreadData* data = new ThreadData(&tid_,callback_,fetch_);
//     if(!pthread_create(&pid_,NULL,threadFunc,data)) {
        
//         MutexGuard lock(mutex_);
//         cond_.wait();
//         assert(tid_ > 0);
//     } else {
//         delete data;
//         data = nullptr;
//     }
// }
// bool Thread::Join() {
//     assert(!join_);
//     join_ = true;
//     return pthread_join(pid_,NULL);
// }
// Thread::~Thread() {
//     if(!join_)
//         pthread_detach(pid_);
// }

// void* threadFunc (void* obj) {
//     ThreadData *data = (ThreadData*)(obj);
//     data->runThreadFunc();
//     delete data;
//     data = nullptr;
//     return NULL;
// }
// eventLoopThread::eventLoopThread(eventLoopThreadCallBack const& cb) 
//     :loop_(0),
//     exit_(false),
//     mutex_(),
//     cond_(mutex_),
//     tcallBack_(cb),
//     thread_(boost::bind(&eventLoopThread::threadFunc,this,_1))
// {
// }

// eventLoopThread::~eventLoopThread() {
//     exit_ = true;
//     if(!loop_) {
//         loop_->quit();
//         thread_.Join();
//     }
// }

// EventLoop* eventLoopThread::startLoop(void) {
    
//     {   //减少临界区的长度
//         MutexGuard lock(mutex_);
//         while(loop_ == nullptr) {
//             cond_.wait();
//         }
        
//     }
//     return loop_;
// }
// void eventLoopThread::threadFunc() {

//     EventLoop loop;
//     {
//         MutexGuard lock(mutex_);
//         loop_ = &loop;
//         cond_.signalSingle();
//     }
//     loop.loop();
// }