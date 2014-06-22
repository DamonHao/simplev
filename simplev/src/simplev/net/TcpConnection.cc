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
		highWaterMark_(64*1024*1024),
		inputBuffer_(),
		outputBuffer_()
 {
	 printf("TcpConnection::ctor[%s], fd= %d\n", name_.c_str(), sockfd);
	 channel_->setReadCallback(
	       boost::bind(&TcpConnection::handleRead, this, _1));
	  channel_->setWriteCallback(
	      boost::bind(&TcpConnection::handleWrite, this));
//	  channel_->setCloseCallback(
//	      boost::bind(&TcpConnection::handleClose, this));
	  channel_->setErrorCallback(
	      boost::bind(&TcpConnection::handleError, this));
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
		handleClose();
	}
	else
	{
		errno = savedErrno;
		Logger::perror("TcpConnection::handleRead");
		handleError();
	}
}


void TcpConnection::handleClose()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnected || state_ == kDisconnecting);
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
	loop_->assertInLoopThread();
	if(channel_->isWriting())
	{
		ssize_t numWrite = sockets::write(channel_->fd(), outputBuffer_.peek(), outputBuffer_.readableBytes());
		if(numWrite > 0)
		{
			outputBuffer_.retrieve(numWrite);
			if(outputBuffer_.readableBytes() == 0)
			{
				channel_->disableWriting();
				if(writeCompleteCallback_)
				{
					//Not call writeCompleteCallback_ directly to reduce time-delay;
					loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
				}
				if(state_ == kDisconnecting)
				{
					shutdownInLoop();
				}
			}
			else
			{
				Logger::puts("TcpConnection::handleWrite: going to write more data");
			}
		}
		else
		{
			Logger::perror("TcpConnection::handleWrite");
		}
	}
	else
	{
		//It happend when: write data to socket while the connection is down.
		//In Channel::handleEvent(), readCallback_ will read socket and return 0, which means
		//connection is down. it will sotp the ioWatcher and disable all events. But it still in
		//Channel::handleEvent(), so the writeCallback_ will be executed. Then it go into this case;
		Logger::puts("Connection is down, no more writing");
	}
}

void TcpConnection::connectDestroyed()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnected || state_ == kDisconnecting);
	setState(kDisconnected);
	channel_->disableAll();
	connectionCallback_(shared_from_this());

//	loop_->removeChannel(get_pointer(channel_));
	channel_->remove();
}

void TcpConnection::shutdownInLoop()
{
  loop_->assertInLoopThread();
  if (!channel_->isWriting())
  {
    // we are not writing
    socket_->shutdownWrite();
  }
}

void TcpConnection::shutdown()
{
	if(state_ == kConnected)
	{
		setState(kDisconnecting);
		//shared_from_this to keep the TcpConnection alive;
		loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, shared_from_this()));
	}
}

void TcpConnection::sendInLoop(const std::string& message)
{
	loop_->assertInLoopThread();
	ssize_t numWrite = 0;
	size_t remaining = message.size();
	bool faultError = false;
	//if nothing in the output buffer, try writing directly
	if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
	{
		//when client close the connection, this cause SIGPIPE and should be ignored.
		numWrite = sockets::write(channel_->fd(), message.data(), message.size());
		if(numWrite  >= 0)
		{
			remaining = remaining - numWrite;
			if(remaining == 0 && writeCompleteCallback_)
			{
					//Not call writeCompleteCallback_ directly to reduce time-delay;
					loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
			}
//			if(implicit_cast<size_t>(numWrite) < message.size())
//			{
//				Logger::puts("going to write more data");
//			}
//			else if(writeCompleteCallback_)
//			{
//				//Not call writeCompleteCallback_ directly to reduce time-delay;
//				loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
//			}
		}
		else
		{
			numWrite = 0;
			if(errno != EWOULDBLOCK)
			{
				Logger::perror("TcpConnection::sendInLoop");
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
			}
		}
	}

	assert(numWrite >= 0);
//	assert(remaining <= len);
//	if(implicit_cast<size_t>(numWrite) < message.size())
//	{
//		outputBuffer_.append(message.data()+numWrite, message.size() - numWrite);
//		if(!channel_->isWriting())
//		{
//			channel_->enableWriting();
//		}
//	}
	if(!faultError && remaining >0)
	{
		 size_t oldLen = outputBuffer_.readableBytes();
		 if(oldLen + remaining >= highWaterMark_
				 && oldLen < highWaterMark_
				 && highWaterMarkCallback_)
		 {
			 loop_->queueInLoop(boost::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
		 }
		 outputBuffer_.append(message.data()+numWrite, remaining);
		 if(!channel_->isWriting())
		 {
			 channel_->enableWriting();
		 }
	}
}

void TcpConnection::send(const std::string& message)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(message);
		}
		else
		{
			loop_->queueInLoop(boost::bind(&TcpConnection::sendInLoop, this, message));
		}
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	socket_->setTcpNoDelay(on);
}

void TcpConnection::setKeepAlive(bool on)
{
	socket_->setKeepAlive(on);
}


