/*
 * test_TcpClient.cc
 *
 *  Created on: Jun 24, 2014
 *      Author: damonhao
 */


#include <stdio.h>
#include <unistd.h>

#include <utility>

#include <boost/bind.hpp>

#include <simplev/net/EventLoop.h>
#include <simplev/net/TcpClient.h>

using namespace simplev;
using namespace simplev::net;

std::string message = "Hello\n";

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(),
           conn->peerAddress().toIpPort().c_str());
    conn->send(message);
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
           conn->name().c_str());
  }
}

void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp receiveTime)
{
  printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toFormattedString().c_str());

  printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main()
{
  EventLoop loop;
  InetAddress serverAddr("localhost", 9981);
  TcpClient client(&loop, serverAddr);

  client.setConnectionCallback(onConnection);
  client.setMessageCallback(onMessage);
  client.enableRetry();
  client.connect();
  loop.loop();
}



