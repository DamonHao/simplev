/*
 * BlockingQueue.h
 *
 *  Created on: Jun 30, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_BLOCKINGQUEUE_H_
#define SIMPLEV_BASE_BLOCKINGQUEUE_H_

#include <assert.h>

#include <deque>

#include <boost/noncopyable.hpp>

#include <simplev/base/Condition.h>

namespace simplev
{

template<typename T>
class BlockingQueue : boost::noncopyable
{
 public:
  BlockingQueue()
    : mutex_(),
      notEmpty_(mutex_),
      queue_()
  {
  }

  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
    notEmpty_.notify(); // wait morphing saves us

  }

  T take()
  {
    MutexLockGuard lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty())
    {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
    T front(queue_.front());
    queue_.pop_front();
    return front;
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable MutexLock mutex_;
  Condition         notEmpty_;
  std::deque<T>     queue_;
};

}

#endif /* SIMPLEV_BASE_BLOCKINGQUEUE_H_ */
