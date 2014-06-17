/*
 * EventLoopThread.cc
 *
 *  Created on: Jun 16, 2014
 *      Author: damonhao
 */

#include <simplev/net/EventLoopThread.h>

#include <boost/bind.hpp>

#include <simplev/net/EventLoop.h>

using namespace simplev::net;

EventLoopThread::EventLoopThread()
: loop_(NULL),
  exiting_(false),
  thread_(boost::bind(&EventLoopThread::threadFunc, this)),
  mutex_(),
  cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  loop_->quit();
  thread_.join();
}

void EventLoopThread::threadFunc()
{
  EventLoop loop;

  {
    muduo::MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
//  assert(exiting_); //test ~EventLoopThread(), and loop_ may quit by other thread call loop_->quit();
  loop_ = NULL;
}

EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();

  {
  	muduo::MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop_;
}

