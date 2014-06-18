/*
 * Socket.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <simplev/net/Socket.h>

#include <strings.h> //bzero()

#include <simplev/net/InetAddress.h>
#include <simplev/net/SocketsOps.h>

using namespace simplev::net;

Socket::~Socket()
{
	sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localaddr)
{
	sockets::bindOrDie(sockfd_, localaddr.getSockAddrInet());
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	// FIXME: CHECK
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
