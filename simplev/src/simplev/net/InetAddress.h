/*
 * InetAddress.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_INETADDRESS_H_
#define SIMPLEV_NET_INETADDRESS_H_

#include <netinet/in.h>

#include <string>

#include <simplev/base/copyable.h>

namespace simplev
{
namespace net
{

/// Wrapper of sockaddr_in.
/// This is an POD interface class.
class InetAddress : simplev::copyable
{
public:
	/// Constructs an endpoint with given port number.
	/// Mostly used in TcpServer listening.
	explicit InetAddress(uint16_t port);


	/// Constructs an endpoint with given ip and port.
	/// @c ip should be "1.2.3.4"
	InetAddress(const std::string& ip, uint16_t port);


	InetAddress(const struct sockaddr_in addr) :
			addr_(addr)
	{
	}
	const struct sockaddr_in& getSockAddrInet() const { return addr_; }
	void setSockAddrInet(const sockaddr_in& addr){ addr_ = addr;}

	std::string toIpPort() const;
private:
	struct sockaddr_in addr_;
};

}
}

#endif /* SIMPLEV_NET_INETADDRESS_H_ */
