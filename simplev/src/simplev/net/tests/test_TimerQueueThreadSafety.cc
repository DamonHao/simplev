/*
 * test_TimerQueueThreadSafety.cc
 *
 *  Created on: Jun 15, 2014
 *      Author: damonhao
 */
#include <stdio.h>

#include <muduo/base/Thread.h>

#include "simplev/net/EventLoop.h"

using namespace simplev::net;

EventLoop* g_loop;

void print()
{
	puts("print()");
	g_loop->quit();
}

void threadFunc()
{
	g_loop->runAfter(2.0, print);
}

int main()
{
	EventLoop loop;
	g_loop = &loop;
	muduo::Thread t(threadFunc);
	t.start();
	loop.loop();
	return 0;
}

