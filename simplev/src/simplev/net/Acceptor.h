/*
 * Acceptor.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_ACCEPTOR_H_
#define SIMPLEV_NET_ACCEPTOR_H_

#include <simplev/net/Socket.h>

#include <ev++.h>
#include <boost/noncopyable.hpp>

namespace simplev
{
namespace net
{
class Acceptor: boost::noncopyable
{
public:
	Acceptor(ev::loop_ref loop, const InetAddress& listenAddr);
	~Acceptor();

private:
	void handleRead();
	Socket acceptSocket_;
	ev::loop_ref loop_;
};

}
}

#endif /* SIMPLEV_NET_ACCEPTOR_H_ */
