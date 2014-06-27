/*
 * test_TcpServerMultiThread.cc
 *
 *  Created on: Jun 23, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include <simplev/net/TcpServer.h>
#include <simplev/net/EventLoop.h>
#include <simplev/net/InetAddress.h>


using namespace simplev;
using namespace simplev::net;

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): tid=%d new connection [%s] from %s\n",
           simplev::CurrentThread::tid(),
           conn->name().c_str(),
           conn->peerAddress().toIpPort().c_str());
  }
  else
  {
    printf("onConnection(): tid=%d connection [%s] is down\n",
           simplev::CurrentThread::tid(),
           conn->name().c_str());
  }
}

void onMessage(const TcpConnectionPtr& conn,
               Buffer* buf,
               Timestamp receiveTime)
{
  printf("onMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
         simplev::CurrentThread::tid(),
         buf->readableBytes(),
         conn->name().c_str(),
         receiveTime.toFormattedString().c_str());

  printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main(int argc, char* argv[])
{
  printf("main(): pid = %d\n", getpid());

  InetAddress listenAddr(9981);
  EventLoop loop;

  TcpServer server(&loop, listenAddr);
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  if (argc > 1) {
    server.setThreadNum(atoi(argv[1]));
  }
  server.start();

  loop.loop();
}



