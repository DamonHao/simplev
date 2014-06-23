/*
 * TcpServer.cc
 *
 *  Created on: Jun 10, 2014
 *      Author: damonhao
 */
#include <simplev/net/TcpServer.h>

#include <poll.h>
#include <stdio.h>

#include <string>

#include <boost/bind.hpp>

#include <simplev/base/Logging.h>
#include <simplev/net/Acceptor.h>
#include <simplev/net/EventLoop.h>
#include <simplev/net/EventLoopThreadPool.h>
#include <simplev/net/InetAddress.h>
//#include <simplev/net/TcpConnection.h>
#include <simplev/net/SocketsOps.h>


using namespace simplev::base;
using namespace simplev::net;

TcpServer::TcpServer(EventLoop *loop, const InetAddress& listenAddr) :
	loop_(checkNotNULL(loop)),
	name_(listenAddr.toIpPort()),
	acceptor_(new Acceptor(loop, listenAddr)),
	threadPool_(new EventLoopThreadPool(loop)),
	started_(false),
	nextConnId_(1)
{
	acceptor_->setNewConnectionCallback(boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
	if(started_ != true)
	{
		started_ = true;
		threadPool_->start();
	}
	if(!acceptor_->listening())
	{
		//Acceptor::listen must run in loop
		loop_->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
	}
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
	loop_->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", nextConnId_);
	++nextConnId_;
	std::string connName = name_ + buf;
	//Logger
	printf("TcpServer::newConnection[%s] from new connection %s\n", name_.c_str(),
			connName.c_str());
	InetAddress localAddr(sockets::getLocalAddr(sockfd));
	//poll with zero timeout to double confirm the new connection
	struct pollfd pfds[1];
	pfds[0].fd = sockfd;
	pfds[0].events = POLLOUT;
	if(::poll(pfds, 1, 0) < 0)
	{
		Logger::perror("TcpServer::newConnection");
		//FIXME: who clean sockfd??
		return;
	}
	EventLoop* ioLoop = threadPool_->getNextLoop();
	TcpConnectionPtr conn(
				new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(boost::bind(&TcpServer::removeConnection, this, _1));
//	conn->connectEstablished();
  //note, TcpConnection::connectEstablished must called in ioLoop;
  ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	//FIXME: why unsafe;
	loop_->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
//	loop_->assertInLoopThread();
//	printf("TcpServer::removeConnection[%s] connection: %s\n", name_.c_str(), conn->name().c_str());
//	size_t n = connections_.erase(conn->name());
//	assert(n == 1);
//	//must use queueInLoop to realease the Channel's watcher
//	loop_->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	loop_->assertInLoopThread();
	printf("TcpServer::removeConnection[%s] connection: %s\n", name_.c_str(), conn->name().c_str());
	size_t n = connections_.erase(conn->name());
	assert(n == 1);
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::setThreadNum(int numThreads)
{
	assert(numThreads >= 0);
	threadPool_->setThreadNum(numThreads);
}
