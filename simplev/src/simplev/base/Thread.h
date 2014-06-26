/*
 * Thread.h
 *
 *  Created on: Jun 26, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_THREAD_H_
#define SIMPLEV_BASE_THREAD_H_

#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

#include <simplev/base/Atomic.h>

namespace simplev
{
class Thread : boost::noncopyable
{
public:
	typedef boost::function<void ()> ThreadFunc;

  explicit Thread(const ThreadFunc&, const std::string& name = std::string());
  ~Thread();

  void start();
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return *tid_; }
  const std::string& name() const { return name_; }

private:
 bool        started_;
 bool        joined_;
 pthread_t   pthreadId_;
 boost::shared_ptr<pid_t> tid_;
 ThreadFunc  func_;
 std::string name_;

 static AtomicInt32 numCreated_;
};
}




#endif /* SIMPLEV_BASE_THREAD_H_ */
