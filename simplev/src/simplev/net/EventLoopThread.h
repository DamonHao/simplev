/*
 * EventLoopThread.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOPTHREAD_H_
#define SIMPLEV_NET_EVENTLOOPTHREAD_H_

//#include <muduo/base/Condition.h>
//#include <muduo/base/Mutex.h>
//#include <muduo/base/Thread.h>

#include <simplev/base/Condition.h>
#include <simplev/base/Mutex.h>
#include <simplev/base/Thread.h>


namespace simplev
{
namespace net
{

class EventLoop;

class EventLoopThread
{
public:
	EventLoopThread();
    ~EventLoopThread();
    EventLoop* startLoop();

private:
	void threadFunc();

	EventLoop* loop_;
	bool exiting_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;

};

}
}

#endif /* SIMPLEV_NET_EVENTLOOPTHREAD_H_ */
