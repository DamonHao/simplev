/*
 * SocketsOps.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <simplev/net/SocketsOps.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h> //readv()
#include <unistd.h>

#include <simplev/base/Logging.h>

//define __USE_GNU for accept4()
#ifndef __USE_GNU
#define __USE_GNU
#endif

using namespace simplev::base;
using namespace simplev::net;

namespace
{
typedef struct sockaddr SA;

const SA* sockaddr_cast(const struct sockaddr_in* addr)
{
	return static_cast<const SA*>(implicit_cast<const void*>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr)
{
	return static_cast<SA*>(implicit_cast<void*>(addr));
}
}

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

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
	int ret = ::bind(sockfd,
			static_cast<const struct sockaddr*>(static_cast<const void*>(&addr)),
			static_cast<socklen_t>(sizeof addr));
	if (ret < 0)
	{
		Logger::perrorAndAbort("sockets::bindOrDie");
	}
}

void sockets::listenOrDie(int sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
	if (ret < 0)
	{
		Logger::perrorAndAbort("sockets::listenOrDie");
	}
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
	int connfd = ::accept4(sockfd,
			static_cast<struct sockaddr*>(static_cast<void*>(addr)), &addrlen,
			SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (connfd < 0)
	{
		int savedErrno = errno;
		Logger::perror("Socket::accept");
		switch (savedErrno)
		{
			case EAGAIN:
			case ECONNABORTED:
			case EINTR:
			case EPROTO: // ???
			case EPERM:
			case EMFILE: // per-process lmit of open file desctiptor ???
				// expected errors
				errno = savedErrno;
				break;
			case EBADF:
			case EFAULT:
			case EINVAL:
			case ENFILE:
			case ENOBUFS:
			case ENOMEM:
			case ENOTSOCK:
			case EOPNOTSUPP:
				// unexpected errors
				Logger::perrorAndAbort("unexpected error of ::accept ");
				break;
			default:
				Logger::perrorAndAbort("unknown error of ::accept ");
				break;
		}
	}
	return connfd;
}

void sockets::shutdownWrite(int sockfd)
{
	if (::shutdown(sockfd, SHUT_WR) < 0)
	{
		Logger::perrorAndAbort("sockets::shutdownWrite");
	}
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
	return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
	return ::write(sockfd, buf, count);
}

void sockets::close(int fd)
{
	if (::close(fd) < 0)
	{
		Logger::perror("sockets::close");
	}
}

void sockets::fromHostPort(const char* ip, uint16_t port,
		struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{
		Logger::perrorAndAbort("sockets::fromHostPort");
	}
}

void sockets::toIpPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
	assert(size >= INET_ADDRSTRLEN);
	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
	size_t end = ::strlen(buf);
	uint16_t port = sockets::networkToHost16(addr.sin_port);
	assert(size > end);
	snprintf(buf + end, size - end, ":%u", port);
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = sizeof(localaddr);
	if (::getsockname(sockfd,
			static_cast<struct sockaddr*>(static_cast<void*>(&localaddr)), &addrlen)
			< 0)
	{
		Logger::perrorAndAbort("sockets::getLocalAddr");
	}
	return localaddr;
}

int sockets::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = sizeof optval;

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
	{
		return errno;
	}
	else
	{
		return optval;
	}
}

int sockets::connect(int sockfd, const struct sockaddr_in& addr)
{
//	return ::connect(sockfd,
//			static_cast<struct sockaddr*>(static_cast<void*>(&addr)), sizeof addr);
	return ::connect(sockfd,sockaddr_cast(&addr), sizeof addr);
}

struct sockaddr_in sockets::getPeerAddr(int sockfd)
{
	struct sockaddr_in peeraddr;
	bzero(&peeraddr, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
	{
		Logger::perror("sockets::getPeerAddr");
	}
	return peeraddr;
}

bool sockets::isSelfConnect(int sockfd)
{
	struct sockaddr_in localaddr = getLocalAddr(sockfd);
	struct sockaddr_in peeraddr = getPeerAddr(sockfd);
	return localaddr.sin_port == peeraddr.sin_port
			&& localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
}
