/*
 * test_TcpConnection_send2.cc
 *
 *  Created on: Jun 21, 2014
 *      Author: damonhao
 */




#include "simplev/base/Timestamp.h"
#include "simplev/net/TcpServer.h"
#include "simplev/net/EventLoop.h"
#include "simplev/net/InetAddress.h"
#include <stdio.h>

using namespace simplev;
using namespace simplev::net;

std::string message1;
std::string message2;
int sleepSeconds = 5;

void onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		printf("onConnection(): new connection [%s] from %s\n",
				conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
    if (sleepSeconds > 0)
    {
      ::sleep(sleepSeconds);
    }

		conn->send(message1);
		conn->send(message2);
		conn->shutdown();
	}
	else
	{
		printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
	}
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
	printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
			buf->readableBytes(), conn->name().c_str(),
			receiveTime.toFormattedString().c_str());

	buf->retrieveAll();
}

int main(int argc, char* argv[])
{
	printf("main(): pid = %d\n", getpid());

	int len1 = 100;
	int len2 = 200;

	if (argc > 2)
	{
		len1 = atoi(argv[1]);
		len2 = atoi(argv[2]);
	}

	message1.resize(len1);
	message2.resize(len2);
	std::fill(message1.begin(), message1.end(), 'A');
	std::fill(message2.begin(), message2.end(), 'B');

	InetAddress listenAddr(9981);
	EventLoop loop;

	TcpServer server(&loop, listenAddr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.start();

	loop.loop();
}
