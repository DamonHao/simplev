/*
 * EventLoop.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/net/EventLoop.h>

#include <assert.h>
#include <sys/eventfd.h>

#include <boost/bind.hpp>

#include <simplev/net/Channel.h>
#include <simplev/base/Logging.h>
#include <simplev/net/SocketsOps.h>
#include <simplev/net/TimerQueue.h>

using namespace simplev;
using namespace simplev::net;
using namespace simplev::base;

namespace
{
__thread EventLoop* t_loopInThisThread = 0;
int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		LOG_SYSERR << "Failed in eventfd";
		abort();
	}
	return evtfd;
}

class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
    LOG_DEBUG <<"Ignore SIGPIPE";
  }
};

IgnoreSigPipe initObj;

}

EventLoop::EventLoop() :
		looping_(false),
		quit_(false),
		evLoop_(),
		prepareWatcher_(evLoop_),
		callingPendingFunctors_(false),
		threadId_(CurrentThread::tid()),
		timerQueue_(new TimerQueue(this)),
		wakeupFd_(createEventfd()),
		wakeupChannel_(new Channel(this, wakeupFd_))
{
	if (t_loopInThisThread)
	{
		LOG_FATAL << "Another EventLoop " << t_loopInThisThread
		              << " exists in this thread " << threadId_;
	}
	else
	{
		t_loopInThisThread = this;
	}
	wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{

}

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	prepareWatcher_.set<EventLoop, &EventLoop::prepareCallBack>(this);
	prepareWatcher_.start();
	evLoop_.run();
	looping_ = false;
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	return timerQueue_->addTimer(cb, delay, 0.0);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	return timerQueue_->addTimer(cb, 0.0, interval);
}

void EventLoop::prepareCallBack()
{
	timerQueue_->clearExpiredTimer();
	doPendingFunctors();
	//execute doPendingFunctors() after last iteration;
	if(quit_ == true)
	{
		evLoop_.break_loop();
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;
	{
		MutexLockGuard lock(mutex_);
		functors.swap(pendingFunctors_);
	}
	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
	callingPendingFunctors_ = false;
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(cb);
	}
	if (!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if (isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

//void EventLoop::quitInLoop()
//{
//	assertInLoopThread();
//	evLoop_.break_loop();
//	//execute the pending functors after last iteration
//	//which has processed all outstanding events
//	doPendingFunctors();//Error: May casuse Channel release itself in its call back;
//}

//improve the thread safety;
//void EventLoop::quit()
//{
//	if (isInLoopThread())
//	{
//		quitInLoop();
//	}
//	else
//	{
//		queueInLoop(boost::bind(&EventLoop::quitInLoop, this));
//	}
//}

void EventLoop::quit()
{
  quit_ = true;
  if (!isInLoopThread())
  {
    wakeup();
  }
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
//	assertInLoopThread();
	channel->stop();
}

void EventLoop::cancel(TimerId timerId)
{
  return timerQueue_->cancel(timerId);
}
