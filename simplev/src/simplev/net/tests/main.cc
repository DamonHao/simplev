/*
 * main.cc
 *
 *  Created on: Jun 10, 2014
 *      Author: damonhao
 */

#include <stdio.h>
//#include <sys/timerfd.h>

//#include "simplev/net/Channel.h"
#include "simplev/net/EventLoop.h"

using namespace simplev::net;

//EventLoop *  g_loop;
//
//void timeout()
//{
//  printf("Timeout!\n");
//  g_loop->quit();
//}

int main()
{
  EventLoop loop1;
//  EventLoop loop2;
	puts("haha");
	loop1.loop();
	puts("xixi");
//  g_loop = &loop;
//
//  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
//  Channel channel(&loop, timerfd);
//  channel.setReadCallback(timeout);
//  channel.enableReading();
//
//  struct itimerspec howlong;
//  bzero(&howlong, sizeof howlong);
//  howlong.it_value.tv_sec = 2;
//  ::timerfd_settime(timerfd, 0, &howlong, NULL);
//  loop.loop();
//  ::close(timerfd);

  return 0;
}
