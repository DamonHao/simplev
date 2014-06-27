/*
 * Condition.h
 *
 *  Created on: Jun 27, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_CONDITION_H_
#define SIMPLEV_BASE_CONDITION_H_

#include <errno.h>
#include <pthread.h>

#include <boost/noncopyable.hpp>

#include <simplev/base/Mutex.h>

namespace simplev
{
class Condition: boost::noncopyable
{
public:
  explicit Condition(MutexLock& mutex) : mutex_(mutex)
  {
    pthread_cond_init(&pcond_, NULL);
  }

  ~Condition()
  {
    pthread_cond_destroy(&pcond_);
  }

  void wait()
  {
    pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
  }

  // returns true if time out, false otherwise.
  bool waitForSeconds(int seconds)
  {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += seconds;
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
  }

  void notify()
  {
    pthread_cond_signal(&pcond_);
  }

  void notifyAll()
  {
    pthread_cond_broadcast(&pcond_);
  }

private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}

#endif /* SIMPLEV_BASE_CONDITION_H_ */
