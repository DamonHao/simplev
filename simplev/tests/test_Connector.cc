/*
 * test_Connector.cc
 *
 *  Created on: Jun 24, 2014
 *      Author: damonhao
 */

#include "simplev/net/Connector.h"
#include "simplev/net/EventLoop.h"

#include <stdio.h>

using namespace simplev::net;

EventLoop* g_loop;

void connectCallback(int sockfd)
{
  printf("connected.\n");
  g_loop->quit();
}

int main(int argc, char* argv[])
{
  EventLoop loop;
  g_loop = &loop;
  InetAddress addr("127.0.0.1", 9981);
  ConnectorPtr connector(new Connector(&loop, addr));
  connector->setNewConnectionCallback(connectCallback);
  connector->start();

  loop.loop();
}


