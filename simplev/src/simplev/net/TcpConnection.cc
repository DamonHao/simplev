/*
 * TcpConnection.cc
 *
 *  Created on: Jun 18, 2014
 *      Author: damonhao
 */
#include <simplev/net/TcpConnection.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include <string>

#include <boost/bind.hpp>

#include <simplev/base/Logging.h>
#include <simplev/net/Channel.h>
#include <simplev/net/EventLoop.h>
#include <simplev/net/Socket.h>
#include <simplev/net/SocketsOps.h>

//using namespace	simplev;
using namespace simplev::base;
using namespace simplev::net;

 TcpConnection::TcpConnection(EventLoop *loop,
                const std::string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr)
:		loop_(loop),
		name_(name),
		state_(kConnecting),
		socket_(new Socket(sockfd)),
		channel_(new Channel(loop, sockfd)),
		localAddr_(localAddr),
		peerAddr_(peerAddr),
		inputBuffer_()
 {
	 printf("TcpConnection::ctor[%s], fd= %d\n", name_.c_str(), sockfd);
	 channel_->setReadCallback(
	       boost::bind(&TcpConnection::handleRead, this, _1));
 }

 TcpConnection::~TcpConnection()
 {
	 printf("TcpConnection::ctor[%s], fd= %d\n", name_.c_str(), socket_->fd());
 }

void TcpConnection::connectEstablished()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnecting);
	setState(kConnected);
	channel_->enableReading();
	connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead(Timestamp receiveTime )
{
	int savedErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
	if(n > 0)
	{
		messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
	}
	else if(n == 0)
	{
//		sockets::close(channel_->fd());
		errno = savedErrno;
		Logger::perror("TcpConnection::handleRead");
		handleClose();
	}
	else
	{
		handleError();
		Logger::perrorAndAbort("TcpConnection::handleRead");
	}
}


void TcpConnection::handleClose()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnected);
	//let the dtor close the sockfd
	channel_->disableAll();
	//must be the last line
	closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
  int err = sockets::getSocketError(channel_->fd());
  Logger::strerror_tl(err); // thread local
}

void TcpConnection::handleWrite()
{

}

void TcpConnection::connectDestroyed()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnected);
	setState(kDisconnected);
	channel_->disableAll();
	connectionCallback_(shared_from_this());

//	loop_->removeChannel(get_pointer(channel_));
	channel_->remove();
}

