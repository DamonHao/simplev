/*
 * EventLoopThreadPool.h
 *
 *  Created on: Jun 23, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_EVENTLOOPTHREADPOOL_H_
#define SIMPLEV_NET_EVENTLOOPTHREADPOOL_H_

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace simplev
{
namespace net
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable
{
public:
  EventLoopThreadPool(EventLoop* baseLoop);
  ~EventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start();
  // valid after calling start()
  EventLoop* getNextLoop();
  // valid after calling start()
  std::vector<EventLoop*> getAllLoops();

private:

	EventLoop* baseLoop_;
	bool started_;
	int numThreads_;
	int next_;
	boost::ptr_vector<EventLoopThread> threads_;
	std::vector<EventLoop*> loops_;
};

}
}

#endif /* SIMPLEV_NET_EVENTLOOPTHREADPOOL_H_ */
