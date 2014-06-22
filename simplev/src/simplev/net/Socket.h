/*
 * Socket.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_SOCKET_H_
#define SIMPLEV_NET_SOCKET_H_

#include <boost/noncopyable.hpp>

namespace simplev
{
namespace net
{

class InetAddress;

class Socket: boost::noncopyable
{
public:
	explicit Socket(int sockfd) : //FIXME: why explicit?
			sockfd_(sockfd)
	{
	}
	~Socket();

	/// abort if address in use
	void bindAddress(const InetAddress& localaddr);
	/// abort if address in use
	void listen();

	/// On success, returns a non-negative integer that is
	/// a descriptor for the accepted socket, which has been
	/// set to non-blocking and close-on-exec. *peeraddr is assigned.
	/// On error, -1 is returned, and *peeraddr is untouched.
	int accept(InetAddress* peeraddr);

	int fd() {return sockfd_;}

	void shutdownWrite();

  // Enable/disable SO_KEEPALIVE
  void setKeepAlive(bool on);

  // Enable/disable SO_REUSEADDR
  void setReuseAddr(bool on);

  // Enable/disable SO_REUSEPORT
  void setReusePort(bool on);

	// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
	void setTcpNoDelay(bool on);

private:
	const int sockfd_;
};

}
}

#endif /* SIMPLEV_NET_SOCKET_H_ */
