/*
 * test_Acceptor.cc
 *
 *  Created on: Jun 18, 2014
 *      Author: damonhao
 */

#include "simplev/net/Acceptor.h"
#include "simplev/net/EventLoop.h"
#include "simplev/net/InetAddress.h"
#include "simplev/net/SocketsOps.h"

#include <stdio.h>
#include <unistd.h>

using namespace simplev::net;

void newConnection(int sockfd, const InetAddress& peerAddr)
{
  printf("newConnection(): accepted a new connection from %s\n",
         peerAddr.toIpPort().c_str());
  ::write(sockfd, "How are you?\n", 13);
  sockets::close(sockfd);
}

int main()
{
  printf("main(): pid = %d\n", getpid());

  InetAddress listenAddr(9981);
  EventLoop loop;

  Acceptor acceptor(&loop, listenAddr);
  acceptor.setNewConnectionCallback(newConnection);
  acceptor.listen();

  loop.loop();
}


