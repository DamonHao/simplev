/*
 * Socket.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <simplev/net/Socket.h>

#include <strings.h> //bzero()
#include <netinet/in.h> //Internet address family
#include <netinet/tcp.h> // TCP_NODELAY

#include <simplev/base/Logging.h>
#include <simplev/net/InetAddress.h>
#include <simplev/net/SocketsOps.h>

using namespace simplev::base;
using namespace simplev::net;

Socket::~Socket()
{
	sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localaddr)
{
	sockets::bindOrDie(sockfd_, localaddr.getSockAddrInet());
}



void Socket::listen()
{
	sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof addr);
	int connfd = sockets::accept(sockfd_, &addr);
	if (connfd >= 0)
	{
		peeraddr->setSockAddrInet(addr);
	}
	return connfd;
}

void Socket::shutdownWrite()
{
  sockets::shutdownWrite(sockfd_);
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	 int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	 if(ret < 0)
	 {
		 LOG_SYSERR << "Socket::setReuseAddr failed.";
	 }
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0)
  {
    LOG_SYSERR << "SO_REUSEPORT failed.";
  }
#else
  if (on)
  {
  	LOG_ERROR << "SO_REUSEPORT is not supported.";
  }
#endif
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
							 &optval, static_cast<socklen_t>(sizeof optval));
	if(ret < 0)
	{
		LOG_SYSERR << "Socket::setTcpNoDelay failed.";
	}
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
	if(ret < 0)
	{
		LOG_SYSERR << "Socket::setKeepAlive failed.";
	}
}
