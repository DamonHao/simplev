/*
 * Thread.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/base/CurrentThread.h>
#include <simplev/base/Thread.h>

#include <assert.h>
#include <unistd.h>
#include <stdio.h> // snprintf()
#include <sys/prctl.h>
#include <sys/syscall.h>

#include <boost/weak_ptr.hpp>

#include <simplev/base/Exception.h>

namespace simplev
{
namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread const char* t_threadName = "unknown";

}

namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

struct ThreadData
{
  typedef simplev::Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  std::string name_;
  boost::weak_ptr<pid_t> wkTid_;

  ThreadData(const ThreadFunc& func,
             const std::string& name,
             const boost::shared_ptr<pid_t>& tid)
    : func_(func),
      name_(name),
      wkTid_(tid)
  { }

  void runInThread()
  {
    pid_t tid = simplev::CurrentThread::tid();
    boost::shared_ptr<pid_t> ptid = wkTid_.lock();

    if (ptid)
    {
      *ptid = tid;
      ptid.reset();
    }

    simplev::CurrentThread::t_threadName = name_.empty() ? "simplevThread" : name_.c_str();
    ::prctl(PR_SET_NAME, simplev::CurrentThread::t_threadName); // set thread's name;
    try
    {
      func_();
      simplev::CurrentThread::t_threadName = "finished";
    }
    catch (const Exception& ex)
    {
      simplev::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
      abort();
    }
    catch (const std::exception& ex)
    {
      simplev::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      abort();
    }
    catch (...)
    {
      simplev::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
      throw; // rethrow
    }
    simplev::CurrentThread::t_threadName = "finished";
  }
};

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}


}
}

using namespace simplev;

void CurrentThread::cacheTid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = detail::gettid();
		int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
		assert(n == 6);	(void) n;
	}
}

bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(new pid_t(0)),
    func_(func),
    name_(n)
{
  numCreated_.increment();
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;

  detail::ThreadData* data = new detail::ThreadData(func_, name_, tid_);
  if (pthread_create(&pthreadId_, NULL, &detail::startThread, data))
  {
    started_ = false;
    delete data;
    abort();
  }
}

int Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}
