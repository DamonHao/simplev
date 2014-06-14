/*
 * EventLoop.h
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOP_H_
#define SIMPLEV_NET_EVENTLOOP_H_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <ev++.h>

#include <simplev/base/CurrentThread.h>
#include <simplev/net/Callbacks.h>
#include <simplev/net/TimerId.h>

namespace simplev
{
namespace net
{

class TimerQueue;

class EventLoop: boost::noncopyable
{
public:
	EventLoop();
	~EventLoop();//make it out-line because boost::scoped_ptr<TimerQueue> timerQueue_;
	void loop();
	void abortNotInLoopThread();
  void assertInLoopThread()
  {
    if (!isInLoopThread())
    {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  ev::loop_ref getEventLoopRef() {return evLoop_;}
  void quit(){evLoop_.break_loop();};

  // Runs callback after @c delay seconds.
  TimerId runAfter(double delay, const TimerCallback& cb);
  // Runs callback every @c interval seconds.
  TimerId runEvery(double interval, const TimerCallback& cb);
private:
	ev::default_loop evLoop_;
	bool looping_; /* atomic */
	const pid_t threadId_;
	boost::scoped_ptr<TimerQueue> timerQueue_;
};

}
}

#endif /* SIMPLEV_NET_EVENTLOOP_H_ */
