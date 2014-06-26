/*
 * test_Channel.cc
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#include <stdio.h>
#include <sys/timerfd.h>

#include "simplev/net/Channel.h"
#include "simplev/net/EventLoop.h"

using namespace simplev::net;

EventLoop *  g_loop;


//void timeout()
//{
//  printf("Timeout!\n");
//  g_loop->quit();
//}

void timeout(simplev::Timestamp receiveTime)
{
  printf("Timeout!\n");
  g_loop->quit();
}

int main()
{
  EventLoop loop;
  g_loop = &loop;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  Channel channel(&loop, timerfd);
  channel.setReadCallback(timeout);
  channel.enableReading();

  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 2;
  ::timerfd_settime(timerfd, 0, &howlong, NULL);
  loop.loop();
  ::close(timerfd);

  return 0;
}


