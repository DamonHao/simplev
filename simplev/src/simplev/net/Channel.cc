/*
 * Channel.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/net/Channel.h>
#include <simplev/net/EventLoop.h>

using namespace ev;
using namespace simplev::net;

Channel::Channel(EventLoop* loop, int fd) :
		loop_(loop), fd_(fd), ioWatcher_(loop->getEventLoopRef())
{
	ioWatcher_.set<Channel, &Channel::handleEvent>(this);
	ioWatcher_.start(fd_, NONE);
}

void Channel::handleEvent(ev::io &io_watcher, int revents)
{
	if (revents & ERROR)
	{
		if (errorCallback_)
		{
			errorCallback_();
		}
	}
	if (revents & READ)
	{
		if (readCallback_)
		{
			readCallback_();
		}
	}
	if (revents & WRITE)
	{
		if (writeCallback_)
		{
			writeCallback_();
		}
	}
}

