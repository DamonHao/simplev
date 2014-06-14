/*
 * TimerQueue.cc
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#include <simplev/net/TimerQueue.h>

//#include <simplev/net/EventLoop.h>
#include <simplev/net/Timer.h>
#include <simplev/net/TimerId.h>

using namespace simplev::net;

TimerQueue::TimerQueue(EventLoop* loop) :
		loop_(loop), timers_()
{
}

TimerQueue::~TimerQueue()
{
	for (TimerList::iterator it = timers_.begin(); it != timers_.end(); ++it)
	{
		delete *it;
	}
}

TimerId TimerQueue::addTimer(const TimerCallback& cb, double after,
		double interval)
{
	Timer * timer = new Timer(loop_, cb, after, interval);
	timer->start();
	timers_.insert(timer);
	return TimerId(timer);
}

void TimerQueue::cancel(TimerId timerId)
{
	timerId.timer_->stop();
//	timers_.erase(timerId.timer_); //FIXME: It is the right time to delete?
//	delete timerId.timer_;
}
