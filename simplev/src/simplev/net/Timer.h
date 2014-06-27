/*
 * Timer.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TIMER_H_
#define SIMPLEV_NET_TIMER_H_

#include <boost/noncopyable.hpp>
#include <ev++.h>

#include <simplev/base/Atomic.h>

#include <simplev/net/Callbacks.h>

namespace simplev
{
namespace net
{
class EventLoop;

///
/// Internal class for timer event.
///
class Timer : boost::noncopyable
{
public:
	Timer(EventLoop *loop, const TimerCallback& cb, double after, double interval);
	~Timer();
	void start();
	void stop();
	bool isActive(){return timeWatcher_.is_active();}
	int64_t sequence() const { return sequence_; }
private:
	void run() {callBack_();} //as a call back run should be non-const
	ev::timer timeWatcher_;
	const int64_t sequence_;
	TimerCallback callBack_;

	static AtomicInt64 s_numCreated_;
};



}
}

#endif /* SIMPLEV_NET_TIMER_H_ */
