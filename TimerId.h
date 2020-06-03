#ifndef MUDUO_NET_TIMERID_H
#define MUDUO_NET_TIMERID_H

#include "copyable.h"

namespace muduo
{

class Timer;

/// An opaque identifier, for canceling Timer.
class TimerId : public muduo::copyable
{
 public:
  TimerId(Timer* timer = NULL, int64_t seq = 0)
    : timer_(timer),
      sequence_(seq)
  {
  }

  friend class TimerQueue;

 private:
  Timer*      timer_;
  int64_t     sequence_;
};

}

#endif  // MUDUO_NET_TIMERID_H
