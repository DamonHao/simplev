/*
 * TcpServer.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TCPSERVER_H_
#define SIMPLEV_NET_TCPSERVER_H_

#include <ev++.h>
#include <boost/noncopyable.hpp>
namespace simplev
{
namespace net
{

class TcpServer: boost::noncopyable
{
	TcpServer(ev::loop_ref loop, const InetAddress& listenAddr);
};

}
}

#endif /* SIMPLEV_NET_TCPSERVER_H_ */
