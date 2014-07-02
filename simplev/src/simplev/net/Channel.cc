/*
 * Channel.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/net/Channel.h>

#include <assert.h>
#include <stdio.h>

#include <simplev/net/EventLoop.h>
#include <simplev/base/Logging.h>

using namespace simplev::net;

Channel::Channel(EventLoop* loop, int fd)
	:loop_(loop),
	 fd_(fd),
	 ioWatcher_(loop->getEventLoopRef()),
	 eventHandling_(false),
	 tied_(false)
{
	ioWatcher_.set<Channel, &Channel::handleEvent>(this);
	ioWatcher_.start(fd_, ev::NONE);
	LOG_TRACE <<"Channel ctor:" << this;
}

Channel::~Channel()
{
	assert(!eventHandling_);
	LOG_TRACE <<"Channel dtor:" << this;
}

void Channel::handleEventWithGuard(ev::io &io_watcher, int revents)
{
	eventHandling_ = true;
	if (revents & ev::ERROR)
	{
		if (errorCallback_)
		{
			errorCallback_();
		}
	}
	if (revents & ev::READ)
	{
		if (readCallback_)
		{
			//			readCallback_();
			Timestamp now(loop_->getEventLoopRef().now());
			readCallback_(now);
		}
	}
	if (revents & ev::WRITE)
	{
		if (writeCallback_)
		{
			writeCallback_();
		}
	}
	eventHandling_ = false;
}

void Channel::handleEvent(ev::io &io_watcher, int revents)
{
  boost::shared_ptr<void> guard;
  if (tied_)
  {
    guard = tie_.lock();
    if (guard)
    {
      handleEventWithGuard(io_watcher, revents);
    }
  }
  else
  {
  	handleEventWithGuard(io_watcher, revents);
  }
}

void Channel::remove()
{
	assert(isNoneEvent());
	loop_->removeChannel(this);
}

void Channel::stop()
{
	loop_->assertInLoopThread();
	ioWatcher_.stop();
}

void Channel::tie(const boost::shared_ptr<void>& obj)
{
  tie_ = obj;
  tied_ = true;
}

