/*
 * TimerQueue.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TIMERQUEUE_H_
#define SIMPLEV_NET_TIMERQUEUE_H_

#include <set>

#include <boost/noncopyable.hpp>
//#include <ev++.h>

#include <simplev/net/Callbacks.h>

namespace simplev
{
namespace net
{
class EventLoop;
class Timer;
class TimerId;

class TimerQueue: boost::noncopyable
{
public:
	TimerQueue(EventLoop* loop);
	~TimerQueue();
	TimerId addTimer(const TimerCallback& cb, double after, double interval);
	void cancel(TimerId timerId);
	void clearExpiredTimer();
private:
//	typedef std::set<ev::timer*> TimerList;
	typedef std::set<Timer *> TimerList;
	EventLoop* loop_;
	TimerList timers_;

};
}
}

#endif /* SIMPLEV_NET_TIMERQUEUE_H_ */
