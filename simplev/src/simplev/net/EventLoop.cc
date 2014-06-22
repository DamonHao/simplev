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
		Logger::perrorAndAbort("Failed in eventfd");
	}
	return evtfd;
}

class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
    Logger::puts("Ignore SIGPIPE");
  }
};

IgnoreSigPipe initObj;

}

EventLoop::EventLoop() :
		evLoop_(), prepareWatcher_(evLoop_), looping_(false), callingPendingFunctors_(
				false), threadId_(CurrentThread::tid()), timerQueue_(
				new TimerQueue(this)), wakeupFd_(createEventfd()), wakeupChannel_(
				new Channel(this, wakeupFd_))
{
	if (t_loopInThisThread)
	{
		Logger::printAndAbort("Another EventLoop exists in this thread");
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
	Logger::printAndAbort("EventLoop::abortNotInLoopThread");
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
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		Logger::printAndAbort("EventLoop::handleRead() haven't reads 8 bytes");
//    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		Logger::printAndAbort("EventLoop::wakeup() haven't write 8 bytes");
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;
	{
		muduo::MutexLockGuard lock(mutex_);
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
		muduo::MutexLockGuard lock(mutex_);
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

void EventLoop::quitInLoop()
{
	assertInLoopThread();
	evLoop_.break_loop();
}

//improve the thread safety;
void EventLoop::quit()
{
	if (isInLoopThread())
	{
		quitInLoop();
	}
	else
	{
		queueInLoop(boost::bind(&EventLoop::quitInLoop, this));
	}
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
//	assertInLoopThread();
	channel->stop();
}
