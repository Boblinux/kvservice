#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

#include <vector>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace muduo
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable
{
 public:
  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start();
  EventLoop* getNextLoop();

 private:
  EventLoop*                          baseLoop_;
  bool                                started_;
  int                                 numThreads_;
  int                                 next_;  // always in loop thread
  boost::ptr_vector<EventLoopThread>  threads_;
  std::vector<EventLoop*>             loops_;
};

}

#endif  // MUDUO_NET_EVENTLOOPTHREADPOOL_H
