/*
 * InetAddress.cc
 *
 *  Created on: Jun 17, 2014
 *      Author: damonhao
 */

#include <simplev/net/InetAddress.h>

#include <strings.h>  // bzero
#include <netinet/in.h>

#include <boost/static_assert.hpp>

#include <simplev/net/SocketsOps.h>

static const in_addr_t kInaddrAny = INADDR_ANY;

using namespace simplev::net;

BOOST_STATIC_ASSERT(sizeof(InetAddress) == sizeof(struct sockaddr_in));

InetAddress::InetAddress(uint16_t port)
{
	bzero(&addr_, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
	addr_.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port)
{
	bzero(&addr_, sizeof addr_);
	sockets::fromHostPort(ip.c_str(), port, &addr_);
}

std::string InetAddress::toIpPort() const
{
  char buf[32];
  sockets::toIpPort(buf, sizeof buf, addr_);
  return buf;
}
