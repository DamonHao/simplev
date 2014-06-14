/*
 * EventLoop.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/net/EventLoop.h>

#include <assert.h>

#include <simplev/base/Logging.h>
#include <simplev/net/TimerQueue.h>

using namespace simplev;
using namespace simplev::net;
using namespace simplev::base;

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() :
		evLoop_(), looping_(false), threadId_(CurrentThread::tid()),
		timerQueue_(new TimerQueue(this))
{
	if (t_loopInThisThread)
	{
		Logger::printAndAbort("Another EventLoop exists in this thread");
	}
	else
	{
		t_loopInThisThread = this;
	}
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
