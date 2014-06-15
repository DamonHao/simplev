/*
 * test_Timer.cc
 *
 *  Created on: Jun 14, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include <boost/bind.hpp>

#include "simplev/net/EventLoop.h"

using namespace simplev::net;

int cnt = 0;
EventLoop* g_loop;

void timerCallBack()
{
	puts("timeout!");
}

void print(const char* msg)
{
  puts(msg);
  if (++cnt == 20)
  {
    g_loop->quit();
  }
}

int main()
{
	EventLoop loop;
	loop.runAfter(3.0, timerCallBack);
	loop.runEvery(2, boost::bind(print, "every2"));
	loop.loop();
	return 0;
}

