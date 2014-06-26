/*
 * test_cancleTimer.cc
 *
 *  Created on: Jun 24, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include <boost/bind.hpp>

#include "simplev/net/EventLoop.h"

using namespace simplev::net;

int cnt = 0;
EventLoop* g_loop;
TimerId g_timerId;

void print(const char* msg)
{
  puts(msg);
  if (++cnt == 2)
  {
    g_loop->cancel(g_timerId);
  }
}

void print2(const char* msg)
{
  puts(msg);
}

int main()
{
	EventLoop loop;
	g_loop = &loop;
	g_timerId = loop.runEvery(2, boost::bind(print, "every2"));
	loop.runEvery(3, boost::bind(print2, "non cancle"));
	loop.loop();
	return 0;
}


