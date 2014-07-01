/*
 * CountDownLatch.cc
 *
 *  Created on: Jun 30, 2014
 *      Author: damonhao
 */

#include <simplev/base/CountDownLatch.h>

using namespace simplev;

CountDownLatch::CountDownLatch(int count)
	:mutex_(),
	 condition_(mutex_),
	 count_(count)
{
}


void CountDownLatch::wait()
{
  MutexLockGuard lock(mutex_);
  while (count_ > 0)
  {
    condition_.wait();
  }
}

void CountDownLatch::countDown()
{
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0)
  {
    condition_.notifyAll();
  }
}

int CountDownLatch::getCount() const
{
  MutexLockGuard lock(mutex_);
  return count_;
}

