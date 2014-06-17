/*
 * EventLoopThread.h
 *
 *  Created on: Jun 16, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOPTHREAD_H_
#define SIMPLEV_NET_EVENTLOOPTHREAD_H_

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>


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
 muduo::Thread thread_;
 muduo::MutexLock mutex_;
 muduo::Condition cond_;

};

}
}

#endif /* SIMPLEV_NET_EVENTLOOPTHREAD_H_ */
