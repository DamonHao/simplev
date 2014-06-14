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
	void start();
	void stop();
private:
	void run() {callBack_();} //as a call back run should be non-const
	ev::timer timeWatcher_;
	TimerCallback callBack_;

};



}
}

#endif /* SIMPLEV_NET_TIMER_H_ */
