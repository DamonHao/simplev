/*
 * SocketsOps.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <sys/socket.h>
#include <unistd.h>

#include <simplev/base/Logging.h>
#include <simplev/net/SocketsOps.h>


using namespace simplev::base;
using namespace simplev::net;

int sockets::createNonblockingOrDie()
{
	int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
			IPPROTO_TCP);
	if (sockfd < 0)
	{
		Logger::perrorAndAbort("sockets::createNonblockingOrDie");
	}
	return sockfd;
}

//void socket::bindOrDie(int sockfd, const struct sockaddr_in& addr)
//{
//	int ret = ::bind(sockfd, static_cast<const struct sockaddr*>(&addr),
//			static_cast<socklen_t>(sizeof addr));
//	if (ret < 0)
//	{
//		Logger::perrorAndAbort("sockets::bindOrDie");
//	}
//}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
  return ::read(sockfd, buf, count);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
  return ::write(sockfd, buf, count);
}
