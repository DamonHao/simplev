/*
 * EventLoop.h
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOP_H_
#define SIMPLEV_NET_EVENTLOOP_H_

#include <boost/noncopyable.hpp>
#include <ev++.h>

#include <simplev/base/CurrentThread.h>

namespace simplev
{
namespace net
{

class EventLoop: boost::noncopyable
{
public:
	EventLoop();
//	~EventLoop();
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
private:
	ev::default_loop evLoop_;
	bool looping_; /* atomic */
	const pid_t threadId_;
};

}
}

#endif /* SIMPLEV_NET_EVENTLOOP_H_ */
