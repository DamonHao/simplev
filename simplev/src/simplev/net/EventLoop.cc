/*
 * EventLoop.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/net/EventLoop.h>

#include <assert.h>

#include <simplev/base/Logging.h>

using namespace simplev;
using namespace simplev::net;
using namespace simplev::base;

EventLoop::EventLoop() :
		evLoop_(), looping_(false), threadId_(CurrentThread::tid())
{

}

void EventLoop::abortNotInLoopThread()
{
	Logger::printAndAbort("EventLoop::abortNotInLoopThread");
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	evLoop_.run();
	looping_ = false;
}

