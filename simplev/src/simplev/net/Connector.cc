/*
 * Connector.cc
 *
 *  Created on: Jun 23, 2014
 *      Author: damonhao
 */
#include <simplev/net/Connector.h>

#include <errno.h>

#include <boost/bind.hpp>

#include <simplev/base/Logging.h>
#include <simplev/net/Channel.h>
#include <simplev/net/EventLoop.h>
#include <simplev/net/SocketsOps.h>

using namespace simplev::base;
using namespace simplev::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr) :
		loop_(loop),
		serverAddr_(serverAddr),
		connect_(false),
		state_(kDisconnected),
		retryDelayMs_(kInitRetryDelayMs)
{
	printf("Connector ctor[%d]", reinterpret_cast<long>(this));
}

Connector::~Connector()
{
  printf("Connector dtor[%d]", reinterpret_cast<long>(this));
  loop_->cancel(timerId_);
  assert(!channel_);
}

void Connector::start()
{
  connect_ = true;
  loop_->runInLoop(boost::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::restart()
{
  loop_->assertInLoopThread();
  setState(kDisconnected);
  retryDelayMs_ = kInitRetryDelayMs;
  connect_ = true;
  startInLoop();
}

void Connector::startInLoop()
{
	loop_->assertInLoopThread();
	assert(state_ == kDisconnected);
	if (connect_)
	{
		connect();
	}
	else
	{
		LOG_DEBUG << "do not connect";
	}
}

void Connector::connect()
{
	int sockfd = sockets::createNonblockingOrDie();
	int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno)
	{
		case 0:
		case EINPROGRESS:
		case EINTR:
		case EISCONN:
			connecting(sockfd);
			break;

		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
			retry(sockfd);
			break;

		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
			LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			break;

		default:
			LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
			sockets::close(sockfd);
			// connectErrorCallback_();
			break;
	}
}

void Connector::connecting(int sockfd)
{
	setState(kConnecting);
	assert(!channel_);
	channel_.reset(new Channel(loop_, sockfd));
	channel_->setWriteCallback(boost::bind(&Connector::handleWrite, this)); //FIXME: unsafe???
	channel_->setErrorCallback(boost::bind(&Connector::handleError, this)); //FIXME: unsafe???

	// channel_->tie(shared_from_this()); is not working,
	// as channel_ is not managed by shared_ptr
	channel_->enableWriting();
}

void Connector::handleWrite()
{
	if (state_ == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = sockets::getSocketError(sockfd);
		if (err)
		{
			printf("Connector::handleWrite - SO_ERROR = ");
			LOG_WARN << "Connector::handleWrite - SO_ERROR = "
			               << err << " " << strerror_tl(err);
			retry(sockfd);
		}
		else if (sockets::isSelfConnect(sockfd))
		{
			LOG_WARN << "Connector::handleWrite - Self connect";
			retry(sockfd);
		}
		else
		{
			setState(kConnected);
			if (connect_)
			{
				newConnectionCallback_(sockfd);
			}
			else
			{
				sockets::close(sockfd);
			}
		}
	}
  else
  {
    // what happened?
    assert(state_ == kDisconnected);
  }
}

void Connector::handleError()
{
  printf("Connector::handleError state= %d", state_);
  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
    retry(sockfd);
  }
}

int Connector::removeAndResetChannel()
{
	channel_->disableAll();
	channel_->remove();
	int sockfd = channel_->fd();
	// Can't reset channel_ here, because we are inside Channel::handleEvent
	loop_->queueInLoop(boost::bind(&Connector::resetChannel, this)); // FIXME: unsafe
	return sockfd;
}

void Connector::resetChannel()
{
	channel_.reset();
}

void Connector::retry(int sockfd)
{
	sockets::close(sockfd);
	setState(kDisconnected);
	if (connect_)
	{
		printf("Connector::retry - Retry connecting to %s in %d milliseconds\n",
				serverAddr_.toIpPort().c_str(), retryDelayMs_);
		timerId_ = loop_->runAfter(retryDelayMs_ / 1000.0,
				boost::bind(&Connector::startInLoop, shared_from_this()));
		retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
	}
	else
	{
		LOG_DEBUG << "do not connect";
	}
}

void Connector::stop()
{
  connect_ = false;
  loop_->queueInLoop(boost::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
  // FIXME: cancel timer
  loop_->cancel(timerId_);
}

void Connector::stopInLoop()
{
	loop_->assertInLoopThread();
	if(state_ == kConnecting)
	{
		setState(kDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

