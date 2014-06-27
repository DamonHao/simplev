/*
 * EventLoop.h
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOP_H_
#define SIMPLEV_NET_EVENTLOOP_H_

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <ev++.h>
//#include <muduo/base/Mutex.h>


#include <simplev/base/CurrentThread.h>
#include <simplev/base/Mutex.h>
#include <simplev/net/Callbacks.h>
#include <simplev/net/TimerId.h>

namespace simplev
{
namespace net
{

class TimerQueue;
class Channel;

class EventLoop: boost::noncopyable
{
public:
	typedef boost::function<void()> Functor;

	EventLoop();
	~EventLoop(); //make it out-line because boost::scoped_ptr<TimerQueue> timerQueue_;
	void loop();
	void abortNotInLoopThread();
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}
	bool isInLoopThread() const
	{
		return threadId_ == CurrentThread::tid();
	}
	ev::loop_ref getEventLoopRef()
	{
		return evLoop_;
	}

//	void quitInLoop();
	void quit();

	// Runs callback after @c delay seconds. Thread safe.
	TimerId runAfter(double delay, const TimerCallback& cb);
	// Runs callback every @c interval seconds. Thread safe.
	TimerId runEvery(double interval, const TimerCallback& cb);
	// Thread safe;
	void cancel(TimerId timerId);

	/// Runs callback immediately in the loop thread.
	/// It wakes up the loop, and run the cb.
	/// If in the same loop thread, cb is run within the function.
	/// Safe to call from other threads.
	void runInLoop(const Functor& cb);
	/// called in other thread
	/// Queues callback in the loop thread.
	/// Runs after finish pooling.
	/// Safe to call from other threads.
	void queueInLoop(const Functor& cb);

	// internal usage
	void wakeup();
	void removeChannel(Channel* channel);

private:
	void prepareCallBack(); // only executed in loop
	void handleRead();  // waked up
	void doPendingFunctors();


	bool looping_; /* atomic */ //FIXME: why atomic?
	bool quit_; /* atomic */
//	ev::default_loop evLoop_;
	ev::dynamic_loop evLoop_;
	ev::prepare prepareWatcher_;
	bool callingPendingFunctors_; /* atomic */
	const pid_t threadId_;
	boost::scoped_ptr<TimerQueue> timerQueue_;
	int wakeupFd_;
	boost::scoped_ptr<Channel> wakeupChannel_;
//	muduo::MutexLock mutex_; //FIXME: change to my own MutexLock;
	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_; // @BuardedBy mutex_
};

}
}

#endif /* SIMPLEV_NET_EVENTLOOP_H_ */
