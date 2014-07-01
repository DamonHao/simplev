/*
 * CountDownLatch.h
 *
 *  Created on: Jun 30, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_COUNTDOWNLATCH_H_
#define SIMPLEV_BASE_COUNTDOWNLATCH_H_

#include <boost/noncopyable.hpp>

#include <simplev/base/Condition.h>
#include <simplev/base/Mutex.h>

namespace simplev
{

class CountDownLatch : boost::noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};

}

#endif /* SIMPLEV_BASE_COUNTDOWNLATCH_H_ */
