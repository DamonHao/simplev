/*
 * test_EventLoopThread.cc
 *
 *  Created on: Jun 17, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include "simplev/net/EventLoop.h"
#include "simplev/net/EventLoopThread.h"

using namespace simplev;
using namespace simplev::net;

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), CurrentThread::tid());
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
         getpid(), muduo::CurrentThread::tid());

  EventLoopThread loopThread;
  EventLoop* loop = loopThread.startLoop();
  loop->runInLoop(runInThread);
  sleep(1);
  loop->runAfter(2, runInThread);
  sleep(3);
  loop->quit();
//  sleep(10);Test quit EventLoopThread from other thread;
  printf("exit main().\n");
}



