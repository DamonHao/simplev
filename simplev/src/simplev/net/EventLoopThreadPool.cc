/*
 * EventLoopThreadPool.cc
 *
 *  Created on: Jun 23, 2014
 *      Author: damonhao
 */

#include <simplev/net/EventLoopThreadPool.h>

#include <simplev/net/EventLoop.h>
#include <simplev/net/EventLoopThread.h>

using namespace simplev::net;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
	:baseLoop_(baseLoop),
	 started_(false),
	 numThreads_(0),
	 next_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
  // Don't delete loop, it's stack variable
}

void EventLoopThreadPool::start()
{
  assert(!started_);
  baseLoop_->assertInLoopThread();

  started_ = true;

  for (int i = 0; i < numThreads_; ++i)
  {
    EventLoopThread* t = new EventLoopThread;
    threads_.push_back(t);
    loops_.push_back(t->startLoop());
  }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	baseLoop_->assertInLoopThread();
	assert(started_);
	EventLoop* loop = baseLoop_;
	if(!loops_.empty())
	{
		//round-robin
		loop = loops_[next_];
		++next_;
		if(next_ >= numThreads_)
		{
			next_ = 0;
		}
	}
	return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
	baseLoop_->assertInLoopThread();
	assert(started_);
	if(loops_.empty())
	{
		return std::vector<EventLoop*>(1, baseLoop_);
	}
	else
	{
		return loops_;
	}
}


