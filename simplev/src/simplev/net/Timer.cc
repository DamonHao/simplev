/*
 * Timer.cc
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include <simplev/net/Timer.h>
#include <simplev/net/EventLoop.h>


using namespace simplev::net;

Timer::Timer(EventLoop *loop, const TimerCallback& cb, double after,
		double interval) :
		timeWatcher_(loop->getEventLoopRef()), callBack_(cb)
{
	printf("Timer: %d\n", reinterpret_cast<int>(this));
	timeWatcher_.set<Timer, &Timer::run>(this);
	timeWatcher_.set(after, interval);
//	timeWatcher_.start(after, interval); //FIXME: Timer in incomplete state??
}

void Timer::start()
{
	timeWatcher_.start();
}

void Timer::stop()
{
	timeWatcher_.stop();
}

Timer::~Timer()
{
	printf("~Timer %d\n", reinterpret_cast<int>(this));
}
