/*
 * test_TcpConnection.cc
 *
 *  Created on: Jun 18, 2014
 *      Author: damonhao
 */

#include <stdio.h>

#include <simplev/net/EventLoop.h>
#include <simplev/net/InetAddress.h>
//#include <simplev/net/TcpConnection.h>
#include <simplev/net/TcpServer.h>

using namespace simplev;
using namespace simplev::net;

void onConnection(const TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		printf("onConnection(): new connection [%s] from %s\n",
				conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
	}
	else
	{
		printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
	}
}

//void onMessage(const TcpConnectionPtr& conn, const char* data, ssize_t len)
//{
//	printf("onMessage(): received %zd bytes from connection [%s]\n", len,
//			conn->name().c_str());
//}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
	printf("onMessage(): received %zd bytes from connection [%s] at %s\n",
			buf->readableBytes(), conn->name().c_str(),
			receiveTime.toFormattedString().c_str());

	printf("onMessage(): [%s]\n", buf->retrieveAsString().c_str());
}

int main()
{
	printf("main(): pid = %d\n", getpid());

	InetAddress listenAddr(9981);
	EventLoop loop;

	TcpServer server(&loop, listenAddr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.start();

	loop.loop();
}

