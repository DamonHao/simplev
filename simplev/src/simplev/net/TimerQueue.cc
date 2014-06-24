/*
 * TimerQueue.cc
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#include <simplev/net/TimerQueue.h>

#include <boost/bind.hpp>

#include <simplev/net/EventLoop.h>
#include <simplev/net/Timer.h>
#include <simplev/net/TimerId.h>

using namespace simplev::net;

TimerQueue::TimerQueue(EventLoop* loop) :
		loop_(loop), validTimers_()
{
}

TimerQueue::~TimerQueue()
{
//	for (TimerList::iterator it = timers_.begin(); it != timers_.end(); ++it)
//	{
//		delete *it;
//	}
	for (ValidTimerSet::iterator it = validTimers_.begin(); it != validTimers_.end(); ++it)
	{
		delete it->first ;
	}
}

TimerId TimerQueue::addTimer(const TimerCallback& cb, double after,
		double interval)
{
//	loop_->assertInLoopThread();
	Timer * timer = new Timer(loop_, cb, after, interval);//thread safe for loop_;
	loop_->runInLoop(boost::bind(&TimerQueue::addTimerInLoop, this, timer));
	return TimerId(timer, timer->sequence());
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
	loop_->assertInLoopThread();
	timer->start();
//	timers_.insert(timer);
	validTimers_.insert(ValidTimer(timer, timer->sequence()));
}

void TimerQueue::cancel(TimerId timerId)
{
	loop_->runInLoop(boost::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
	loop_->assertInLoopThread();
	ValidTimer timer(timerId.timer_, timerId.sequence_);
	ValidTimerSet::iterator it = validTimers_.find(timer);
	if(it != validTimers_.end())
	{
		timerId.timer_->stop(); //Let TimerQueue::clearExpiredTimer clean this inactive timer;
	}
}

void TimerQueue::clearExpiredTimer()
{
	ValidTimerSet::iterator itEnd = validTimers_.end();
	ValidTimerSet::iterator itBegin = validTimers_.begin();
	while(itBegin != itEnd)
	{
		if(itBegin->first->isActive())
		{
			++itBegin;
		}
		else
		{
			delete itBegin->first;
			validTimers_.erase(itBegin++);
		}
	}
}
