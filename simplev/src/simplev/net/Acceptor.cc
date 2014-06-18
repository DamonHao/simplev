/*
 * Acceptor.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <simplev/net/Acceptor.h>

#include <errno.h>
#include <fcntl.h>

#include <boost/bind.hpp>

#include <simplev/net/EventLoop.h>
#include <simplev/net/InetAddress.h>
#include <simplev/net/SocketsOps.h>

using namespace ev;
using namespace simplev::net;

Acceptor::Acceptor(EventLoop * loop, const InetAddress& listenAddr) :
		loop_(loop),
		acceptSocket_(sockets::createNonblockingOrDie()),
		acceptChannel_(loop, acceptSocket_.fd()),
		listening_(false),
		idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(boost::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
	acceptChannel_.disableAll();
//	acceptChannel_.remove();
	sockets::close(idleFd_);
}

void Acceptor::listen()
{
	loop_->assertInLoopThread();
	acceptChannel_.enableReading();
	acceptSocket_.listen();
}

void Acceptor::handleRead()
{
	loop_->assertInLoopThread();
	InetAddress peerAddr(0);
	int connfd = acceptSocket_.accept(&peerAddr);
	if(connfd >= 0)
	{
		if(newConnectionCallback_)
		{
			newConnectionCallback_(connfd, peerAddr);
		}
		else
		{
			sockets::close(connfd);
		}
	}
	else
	{
		//fds have been used out;
		if(errno == EMFILE)
		{
			::close(idleFd_);
			idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
			::close(idleFd_);
			idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}
	}
}

