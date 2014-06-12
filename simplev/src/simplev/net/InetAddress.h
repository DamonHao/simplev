/*
 * InetAddress.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_INETADDRESS_H_
#define SIMPLEV_NET_INETADDRESS_H_

#include <netinet/in.h>

namespace simplev
{
namespace net
{

class InetAddress
{
public:
	InetAddress(const struct sockaddr_in addr) :
			addr_(addr)
	{
	}
	const struct sockaddr_in& getSockAddrInet() const { return addr_; }
private:
	struct sockaddr_in addr_;
};

}
}

#endif /* SIMPLEV_NET_INETADDRESS_H_ */
